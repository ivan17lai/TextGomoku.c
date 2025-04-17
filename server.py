from flask import Flask, request, jsonify
from datetime import datetime

app = Flask(__name__)

BOARD_SIZE = 15
board = [['.' for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)]

LOG_FILE = 'log.txt'

def log_action(action):
    now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open(LOG_FILE, 'a', encoding='utf-8') as f:
        f.write(f"[{now}] {action}\n")

def print_board():
    print("\n   " + " ".join([f"{i:2}" for i in range(BOARD_SIZE)]))
    for i, row in enumerate(board):
        print(f"{i:2} " + "  ".join(row))
    print()

@app.route('/join_game', methods=['GET'])
def join_game():
    pos = request.args.get('v')
    try:
        log_action("加入了遊戲 v"+pos)
    except:
        return jsonify({"error": "請打開新版本"}), 400
    global board
    print("🟢 玩家加入了遊戲")
    board = [['.' for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)]
    return "加入了遊戲", 200

@app.route('/win', methods=['GET'])
def win():
    log_action("🎉 玩家獲勝")
    print("🎉 玩家獲勝")
    return "你贏了！", 200

@app.route('/lose', methods=['GET'])
def lose():
    log_action("💀 玩家失敗")
    print("💀 玩家失敗")
    return "你輸了！", 200

@app.route('/next', methods=['GET'])
def next_move():
    pos = request.args.get('pos')
    if not pos or len(pos) != 2:
        return jsonify({"error": "請提供兩位十六進位的 pos，例如 pos=2A"}), 400

    try:
        row = int(pos[0], 16)
        col = int(pos[1], 16)
    except ValueError:
        return jsonify({"error": "pos 格式錯誤，必須是十六進位"}), 400

    board[row][col] = 'O'
    log_action(f"玩家下棋：({row}, {col})")
    print_board()
    return jsonify({"player": {"row": row, "col": col}}), 200

@app.route('/botnext', methods=['GET'])
def bot_move():
    pos = request.args.get('pos')
    if not pos or len(pos) != 2:
        return jsonify({"error": "請提供兩位十六進位的 pos，例如 pos=2A"}), 400

    try:
        row = int(pos[0], 16)
        col = int(pos[1], 16)
    except ValueError:
        return jsonify({"error": "pos 格式錯誤，必須是十六進位"}), 400

    board[row][col] = 'X'
    log_action(f"電腦下棋：({row}, {col})")
    print_board()
    return jsonify({"bot": {"row": row, "col": col}}), 200

if __name__ == '__main__':
    app.run(debug=True)
