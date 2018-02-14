from flask import Flask, render_template, request
app = Flask(__name__)

import redis

import datetime
import json

@app.route("/")
def hello():
    return "Hello World!"

@app.route("/ajax_get.html")
def ajax_get():
    return render_template('ajax_get.html')

@app.route("/post_test/", methods=['POST'])
def post_test():
    data = request.get_json()
    return json.dumps({'my_number': data['my_number'] * 2})

@app.route("/get_test/")
def get_test():
    return "Hello World!"

@app.route("/guess_start/")
def guess_start():
    return render_template('guess_start.html')

@app.route("/guess/", methods=['GET'])
def guess():
    max_value = request.args.get('max_value', 25)
    user = request.args.get('user', 'User')
    return render_template('guess3.html', max_value=max_value, user=user)

@app.route("/guess/hiscore", methods=['POST'])
def hiscore():
    data = request.get_json()
    return get_hiscore(data['max_value'])

def get_hiscore(key):
    stored_data = r.lrange(key, 0, -1)
    hiscore = [json.loads(s) for s in stored_data]
    hiscore.sort(key=lambda x: x['guesses'])
    return json.dumps(hiscore)

@app.route("/guess/finished", methods=['POST'])
def finished():
    data = request.get_json()
    curr_date = str(datetime.datetime.now())
    r.lpush(str(data['max_value']), json.dumps({'guesses': data['guesses'], 'date': curr_date, 'user': data['user']}))
    return get_hiscore(data['max_value'])

r = redis.StrictRedis(host='localhost', port=6379, db=0)

@app.route("/purge/")
def purge():
    r.flushdb()
    return 'purged'
