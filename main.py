#encoding:utf-8
from flask import Flask,request, Response, jsonify

import os.path
import pdb
import datetime

app = Flask(__name__)

from tools import *
from model import *

g_zis={}
g_cis={}


#首页
@app.route("/")
def index():
    return app.send_static_file('index.html')

#查看记录
@app.route("/view")
def view():
    return app.send_static_file('view.html')

#录入一条新记录，响应多音字选择界面
@app.route("/create",methods=['POST'])
def create():
    params = request.args.to_dict()
    data =request.data.decode()

    sql="insert into log(txt,dt) values({0},{1})".format(data,datetime.datetime.now())
    conn.execute(sql)
    rs=conn.execute("select max(id) as max from log")
    return '{"code":"200","fun":"create","id":'+rs[0].max+"}"

#多音字选择结果上传，响应预览界面
@app.route("/duo")
def duo():
    params = request.args.to_dict()
    form =request.form.to_dict()

    global g_zis
    rs="["
    for row in data.split("换行"):
        rs+='{'
        for ci in row.split("空格"):
            rs+='"'+ci+'":['
            if ci in g_cis:
                rs += '"'+ci+'-'+g_cis[ci]+'",'
            for zi in ci:
                if zi in g_zis:
                    rs += '"'+zi+'-'+g_zis[zi]+'",'
                else:
                    rs += '"'+zi+'-????",'
            rs+='],'
        rs+='},'
    rs+=']'
    rs=rs.replace(",}","}")
    rs=rs.replace(",]","]")
    print(rs)
    return '{"code":"200","fun":"create","rs":'+rs+"}"

    r = obj(result="404",fun="publish")

if __name__ == "__main__":
    app.config['JSON_AS_ASCII'] = False
    #app.config['DEBUG'] = True

    #print( app.url_map )
    urlmap = [" <Rule '{r}' {mtd} -> {ep} >,".format(r=x.rule,mtd=x.methods,ep=x.endpoint) for x in app.url_map._rules]
    urlmap.sort()
    [print(x) for x in urlmap]

    for row in conn.execute("select * from zi").fetchall():
        g_zis[row["id"]]=row["py"]

    for row in conn.execute("select * from ci").fetchall():
        g_cis[row["id"]]=row["py"]

    app.run( host="0.0.0.0",port=80 )
