#encoding:utf-8
from flask import Flask,request, Response, jsonify

import os.path
import pdb
import datetime
import json

app = Flask(__name__)

from tools import *
from model import *

g_zis={}


#首页
@app.route("/")
def index():
    return app.send_static_file('index.html')

#查看记录
@app.route("/view")
def view():
    params = request.args.to_dict()
    form =request.form.to_dict()

    rs=Query("select * from log where id="+params["id"])
    if len(rs) == 0:
        return '{"code":"400","msg":"ID不存在","fun":"create"}'

    #[  [  [{zi:"", py:""},{zi:"", py:""}],
    global g_zis
    jsn="["
    for row in rs[0].txt.split("\n"):
        jsn+='['
        for ci in row.split(" "):
            jsn+='['
            py=Query("select * from ci where id='"+ci+"'")
            if len(py):
                py=py[0].py.split(",")
                for i,zi in enumerate(ci):
                    jsn += '{"zi":"%s","py":"%s"},'%(zi,py[i])
            else:
                for zi in ci:
                    if zi in g_zis:
                        jsn += '{"zi":"%s","py":"%s"},'%(zi,g_zis[zi][0])
                    else:
                        jsn += '{"zi":"%s"},'%zi
            jsn+='],'
        jsn+='],'
    jsn+=']'
    jsn=jsn.replace(",}","}")
    jsn=jsn.replace(",]","]")
    print(jsn)
    return '{"code":"200","fun":"view","rs":'+jsn+"}"

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
            py=Query("select * from ci where id='"+ci+"'")
            if len(py):
                py=py[0].py.split(",")
            for i,e in enumerate(ci):
                jsn += '{"zi":"'+e+'"'
                pl=[]
                if e in g_zis:
                    if len(py):
                        jsn += ',"py":"'+py[i]+'"'
                        pl=[x for x in g_zis[e] if x != py[i]]
                    else:
                        pl=g_zis[e]
                if len(pl)>0:
                    jsn += ',"pl":["'+'","'.join(pl)+'"]},'
                else:
                    jsn += '},'
            jsn+='],'
        jsn+='],'
    jsn+=']'
    jsn=jsn.replace(",}","}")
    jsn=jsn.replace(",]","]")
    print(jsn)
    return '{"code":"200","fun":"create","rs":'+jsn+"}"

#录入一条新记录，响应多音字选择界面
@app.route("/ciwt",methods=['POST'])
def ciwt():
    params = request.args.to_dict()
    vals =request.data.decode()
    for x in vals.split(";"):
        y = x.split(":")
        sql=""
        if querycount("ci",obj(id=y[0]))>0:
            sql="update ci set py='"+y[1]+"' where id='"+y[0]+"'"
        else:    
            sql="insert into ci(id,py) values("+",".join(["'"+z+"'" for z in y])+")"
        conn.execute(sql)
    return '{"code":"200","fun":"ciwt"}'


if __name__ == "__main__":
    jsn = json.loads('["蓝色:lán,sè", "人鱼的:rén,yú,de", "面上:miàn,shàng", "露出:lòu,chū", "蓝色:lán,sè", "似乎:sì,hū", "想要:xiǎng,yāo", "什么:shén,ma"]')
    
    a=[]
    for ci in jsn:
        t=[]
        for x in ci.split(":"):
           t.append( "'"+x+"'")
        a.append("("+",".join(t)+")")
        
        
    zz=",".join(a)
    
    tt = ",".join(['('+ci+")".join(['"'+x+'"' for x in ci.split(":")])+'"' for ci in jsn])

    k = [["'"+x+"'" for x in ci.split(":") ] for ci in jsn ]
    k = ",".join(["("+",".join(ci)+")" for ci in k ])

    kk = ",".join(["("+",".join(["'"+x+"'" for x in ci.split(":") ])+")" for ci in jsn ])



    app.config['JSON_AS_ASCII'] = False
#    app.config['DEBUG'] = True

    #print( app.url_map )
    urlmap = [" <Rule '{r}' {mtd} -> {ep} >,".format(r=x.rule,mtd=x.methods,ep=x.endpoint) for x in app.url_map._rules]
    urlmap.sort()
    [print(x) for x in urlmap]

    for row in conn.execute("select * from zi").fetchall():
        g_zis[row["id"]]=row["py"].split(",")
    if '中' in g_zis:
        print("haha!")

    app.run( host="0.0.0.0",port=80 )
