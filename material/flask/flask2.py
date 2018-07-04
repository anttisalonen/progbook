import subprocess

from flask import Flask, render_template, request
app = Flask(__name__)

@app.route("/", methods=['POST', 'GET'])
def hello():
    if request.method == 'POST':
        if request.form['submit'] == 'Start':
            subprocess.call('killall mpg123', shell=True)
            subprocess.call('mpg123 http://lyd.nrk.no/nrk_radio_p1_trondelag_mp3_h &', shell=True)
        elif request.form['submit'] == 'Stop':
            subprocess.call('killall mpg123', shell=True)
    return render_template('radio.html')

