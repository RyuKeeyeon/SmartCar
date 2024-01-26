# web_control/app.py
from flask import Flask, render_template, request

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/control', methods=['POST'])
def control_car():
    data = request.get_json()
    command = data.get('command')

    # 여기에서 자동차를 제어하는 코드 추가 (car_control.py 참조)

    return 'OK'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
