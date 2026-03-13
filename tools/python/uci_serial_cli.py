"""Interactive host-side bridge for the autonomous chessboard.

This tool validates user moves with python-chess, asks Stockfish for a reply,
and forwards the corresponding motion commands to the STM32 firmware over a
serial link using the board protocol implemented in AutonomousChess_Main.c.
"""

from __future__ import annotations

import argparse
import re
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Optional

import chess
import chess.engine
import serial


START_GAME_BYTE = b" "
COMMAND_TERMINATOR = "$"
COMPLETION_MARKER = "*"
DEFAULT_ENGINE_TIME = 0.1


@dataclass
class MoveHistory:
    undone_moves: List[str] = field(default_factory=list)

    def push_undone(self, move_uci: str) -> None:
        self.undone_moves.append(move_uci)

    def pop_undone(self) -> Optional[str]:
        if not self.undone_moves:
            return None
        return self.undone_moves.pop()

    def clear(self) -> None:
        self.undone_moves.clear()


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Play against Stockfish using the autonomous chessboard firmware protocol.")
    parser.add_argument("--port", default="/dev/ttyACM0", help="Serial port connected to the STM32 board.")
    parser.add_argument("--baudrate", type=int, default=115200, help="Serial baudrate.")
    parser.add_argument("--engine", default="/usr/games/stockfish", help="Path to the Stockfish executable.")
    parser.add_argument("--elo", type=int, default=900, help="Target Stockfish Elo (500-1500 recommended for the legacy setup).")
    parser.add_argument("--side", choices=["w", "b"], default="w", help="Player side: w for white, b for black.")
    parser.add_argument("--serial-timeout", type=float, default=3.0, help="Serial timeout in seconds.")
    return parser.parse_args()


def encode_motion_flags(board: chess.Board, move: chess.Move, move_uci: str) -> str:
    san_move = board.san(move)
    if san_move == "O-O":
        return move_uci + "&"
    if san_move == "O-O-O":
        return move_uci + "@"
    if "x" in san_move:
        return move_uci + "x"
    return move_uci


def send_command(ser: serial.Serial, command: str) -> None:
    if not ser.is_open:
        ser.open()
    payload = (command + COMMAND_TERMINATOR).encode("utf-8", "ignore")
    ser.write(payload)


def wait_for_completion(ser: serial.Serial, timeout_s: float = 10.0) -> None:
    deadline = time.monotonic() + timeout_s
    while time.monotonic() < deadline:
        chunk = ser.read_until(COMPLETION_MARKER.encode("utf-8"), 2).decode("utf-8", "ignore")
        if chunk:
            print(chunk.rstrip())
        if COMPLETION_MARKER in chunk:
            return
        time.sleep(0.1)
    raise TimeoutError("The firmware did not report move completion within the expected time window.")


def print_help_banner() -> None:
    print("Autonomous Chessboard CLI")
    print("Commands: enter moves in SAN, or use undo, redo, quit")
    print("Examples: e4, Nf3, O-O, exd5, e8=Q")


def apply_and_send_engine_move(board: chess.Board, engine: chess.engine.SimpleEngine, limit: chess.engine.Limit, ser: serial.Serial) -> None:
    result = engine.play(board, limit)
    move = result.move
    encoded = encode_motion_flags(board, move, move.uci())
    board.push(move)
    print(f"Engine move: {move.uci()}")
    print(board)
    send_command(ser, encoded)
    wait_for_completion(ser)


def main() -> int:
    args = parse_args()
    if not 500 <= args.elo <= 1500:
        raise SystemExit("--elo must be between 500 and 1500 for this legacy setup.")

    board = chess.Board()
    history = MoveHistory()
    limit = chess.engine.Limit(time=DEFAULT_ENGINE_TIME)

    with serial.Serial(port=args.port, baudrate=args.baudrate, timeout=args.serial_timeout) as ser,              chess.engine.SimpleEngine.popen_uci(args.engine) as engine:
        engine.configure({"UCI_LimitStrength": True, "UCI_Elo": args.elo})
        print_help_banner()

        ser.write(START_GAME_BYTE)
        print(board)

        if args.side == "b":
            apply_and_send_engine_move(board, engine, limit, ser)

        while not board.is_game_over():
            user_input = input("move> ").strip()

            if user_input == "quit":
                break

            if user_input == "undo":
                if board.move_stack:
                    last_move = board.pop().uci()
                    history.push_undone(last_move)
                    reversed_move = last_move[2:4] + last_move[:2]
                    reverse_command = reversed_move
                    try:
                        reverse_move = chess.Move.from_uci(last_move)
                        reverse_command = encode_motion_flags(board, reverse_move, reversed_move)
                    except Exception:
                        pass
                    send_command(ser, reverse_command + "-")
                    wait_for_completion(ser)
                    print(board)
                else:
                    print("No previous move is available.")
                continue

            if user_input == "redo":
                redo_uci = history.pop_undone()
                if redo_uci is None:
                    print("No move is available to redo.")
                    continue
                redo_move = chess.Move.from_uci(redo_uci)
                encoded = encode_motion_flags(board, redo_move, redo_uci)
                board.push(redo_move)
                send_command(ser, encoded)
                wait_for_completion(ser)
                print(board)
                continue

            try:
                user_move = board.parse_san(user_input)
            except ValueError:
                print("Invalid move. Enter a valid SAN move or use undo/redo/quit.")
                continue

            encoded_user_move = encode_motion_flags(board, user_move, user_move.uci())
            board.push(user_move)
            history.clear()
            print(f"Player move: {user_move.uci()}")
            print(board)
            send_command(ser, encoded_user_move)
            wait_for_completion(ser)

            if board.is_game_over():
                break

            apply_and_send_engine_move(board, engine, limit, ser)

    outcome = board.outcome()
    if outcome is not None:
        print(f"Game finished: {outcome}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
