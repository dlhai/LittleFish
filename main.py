#encoding:utf-8
from flask import Flask,request, Response, jsonify

import os.path
import pdb

app = Flask(__name__)

from tools import *
from model import *

#首页
@app.route("/")
def index():
    return app.send_static_file('index.html')

#查看记录
@app.route("/view")
def index():
    return app.send_static_file('view.html')

#多音字处理页面
@app.route("/duo")
def index():
    return app.send_static_file('duo.html')

if __name__ == "__main__":
    app.config['JSON_AS_ASCII'] = False
    #app.config['DEBUG'] = True

    #print( app.url_map )
    urlmap = [" <Rule '{r}' {mtd} -> {ep} >,".format(r=x.rule,mtd=x.methods,ep=x.endpoint) for x in app.url_map._rules]
    urlmap.sort()
    [print(x) for x in urlmap]

    app.run( host="0.0.0.0" )
