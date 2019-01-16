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
    data=data.replace("换行","\n")
    data=data.replace("空格"," ")

    sql="insert into log(txt,dt) values('{0}','{1}')".format(data,datetime.datetime.now())
    conn.execute(sql)
    
    rs=Query("select max(id) as max from log")
    return '{"code":"200","fun":"create","id":'+str(rs[0].max)+"}"


@app.route('/static/duo.html?id=<id>')
def bfile(bdir,bfile):
    return app.send_static_file("duo.html")

#多音字选择结果上传，响应预览界面
@app.route("/duo",methods=['GET', 'POST'])
def duo():
    params = request.args.to_dict()
    form =request.form.to_dict()

    rs=Query("select * from log where id="+params["id"])
    if len(rs) == 0:
        return '{"code":"400","msg":"ID不存在","fun":"create"}'

    #{zi:"", py:"",pl:["","","",""]},
    global g_zis
    jsn="["
    for row in rs[0].txt.split("\n"):
        jsn+='['
        for ci in row.split(" "):
            jsn+='['
            py=Query("select * from ci where id="+ci)
            if len(py):
                py=py[0].py.split(",")
            for i,e in enumerate(ci):
                jsn += '{"zi":"'+e+'"'
                if zi in g_zis:
                    if len(py):
                        jsn += ',"py":"'+py[i]+'",'
                    jsn += ',"pl":["'+'","'.join(g_zis[zi])+'"]},'
                else:
                    jsn += '},'
            jsn+='],'
        jsn+='],'
    jsn+=']'
    jsn=jsn.replace(",}","}")
    jsn=jsn.replace(",]","]")
    print(jsn)
    return '{"code":"200","fun":"create","rs":'+jsn+"}"

if __name__ == "__main__":
    app.config['JSON_AS_ASCII'] = False
#    app.config['DEBUG'] = True

    #print( app.url_map )
    urlmap = [" <Rule '{r}' {mtd} -> {ep} >,".format(r=x.rule,mtd=x.methods,ep=x.endpoint) for x in app.url_map._rules]
    urlmap.sort()
    [print(x) for x in urlmap]

    for row in conn.execute("select * from zi").fetchall():
        g_zis[row["id"]]=row["py"]

    for row in conn.execute("select * from ci").fetchall():
        g_cis[row["id"]]=row["py"]

    app.run( host="0.0.0.0",port=80 )
