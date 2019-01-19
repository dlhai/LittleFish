function int(s) {
    if (s == "") return 0;
    else if (s == "None") return 0;
    else return parseInt(s);
}

//var str = '这是一个测试的字符串：{0} {1}'.format('Hello', 'world');
//var str = '这是一个测试的字符串：{str0} {str1}'.format({ str0: 'Hello', str1: 'world' });
String.prototype.format = function (args) {
    var result = this;
    if (arguments.length > 0) {
        if (arguments.length == 1 && typeof (args) == "object") {
            for (var key in args) {
                if (args[key] != undefined) {
                    var reg = new RegExp("({" + key + "})", "g");
                    result = result.replace(reg, args[key]);
                }
            }
        }
        else {
            for (var i = 0; i < arguments.length; i++) {
                if (arguments[i] != undefined) {
                    var reg = new RegExp("({)" + i + "(})", "g");
                    result = result.replace(reg, arguments[i]);
                }
            }
        }
    }
    return result;
}

//判断是否以某个字符串结尾
String.prototype.endWith = function (endStr) {
    var d = this.length - endStr.length;
    return (d >= 0 && this.lastIndexOf(endStr) == d)
}

// 对Date的扩展，将 Date 转化为指定格式的String   
// 月(M)、日(d)、小时(h)、分(m)、秒(s)、季度(q) 可以用 1-2 个占位符，   
// 年(y)可以用 1-4 个占位符，毫秒(S)只能用 1 个占位符(是 1-3 位的数字)   
// 例子：   
// (new Date()).Format("yyyy-MM-dd hh:mm:ss.S") ==> 2006-07-02 08:09:04.423   
// (new Date()).Format("yyyy-M-d h:m:s.S")      ==> 2006-7-2 8:9:4.18   
Date.prototype.format = function (fmt) { //author: meizz   
    var o = {
        "M+": this.getMonth() + 1,                 //月份   
        "d+": this.getDate(),                    //日   
        "h+": this.getHours(),                   //小时   
        "m+": this.getMinutes(),                 //分   
        "s+": this.getSeconds(),                 //秒   
        "q+": Math.floor((this.getMonth() + 3) / 3), //季度   
        "S": this.getMilliseconds()             //毫秒   
    };
    if (/(y+)/.test(fmt))
        fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    for (var k in o)
        if (new RegExp("(" + k + ")").test(fmt))
            fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
    return fmt;
}

// 对Array的扩展，相当于将Array的filter和map的功能合并
Array.prototype.fmap = function (cb) {
    return this.map(cb).filter(x => x != undefined);
}

// 对Array的扩展
Array.prototype.insertbeforename = function (name, it) {
    idx = GetIdx(this, "name", name);
    if (idx != null)
        this.splice(idx + 1, 0, it);
}

// 对Array的扩展
Array.prototype.insertaftername = function (name, it) {
    idx = GetIdx(this, "name", name);
    if (idx != null)
        this.splice(parseInt(idx) + 1, 0, it);
}

function isInArray(arr, value) {
    for (var i = 0; i < arr.length; i++) {
        if (value === arr[i]) {
            return true;
        }
    }
    return false;
}

function request(url, data, ctx, cb) {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
            res = $.parseJSON(xhr.responseText);
            if (res.code != "200")
                alert("方法：" + url + "\n应答：[" + res.code + "]" + res.msg)
            if (cb != undefined)
                cb(res, ctx);
        }
    };

    if (data != "") {
        xhr.open('POST', url, true);
        xhr.send(data);
    }
    else {
        xhr.open('GET', url, true);
        xhr.send();
    }
}


function GetUrlParam(name) {
    var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)");
    var r = window.location.search.substr(1).match(reg);
    if (r != null) return unescape(r[2]); return null;
}


/*
function Reqdata(url, ctx, fun) {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            json = $.parseJSON(xmlhttp.responseText);
            if (json.result != "200")
                alert("方法：" + url+"\n应答：["+json.result+"]"+json.msg)
            fun(json, ctx);
        }
    };
    xmlhttp.send();
}

function ReqdataP(url, jsn, ctx, cb) {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            res = $.parseJSON(xmlhttp.responseText);
            if (res.result != "200")
                alert("方法：" + url + "\n应答：[" + res.result + "]" + res.msg)
			if (cb != undefined)
				cb(res, ctx);
        }
    };
    xmlhttp.open("POST", url, true);
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    xmlhttp.send(jsn);
}

function Sendform(url, fd, ctx, cb) {
	alert("调用了不严谨的方法Sendform()，请更换为postform");
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) { cb() }
    };

    xhr.open('POST', url, true);
    xhr.send(fd);
}

// 用来代替Sendform
// Sendform的问题是需要外部提前对fd检查以避免响应出错（为了可靠在py中也需要检查），这样的话检查的代码需要实现两次。
// 此版本不对fd的检查，靠py检查，然后对根据响应结果进行处理
function postform(url, fd, ctx, cb) {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) { 
            res = $.parseJSON(xhr.responseText);
            if (res.result != "200")
                alert("方法：" + url + "\n应答：[" + res.result + "]" + res.msg)
			if (cb != undefined)
				cb(res, ctx);
		}
    };

    xhr.open('POST', url, true);
    xhr.send(fd);
}


// 同步方式
function ReqdataS(url, ctx, fun) {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", url, false);
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            json = $.parseJSON(xmlhttp.responseText);
            if (json.result != "200")
                alert("方法：" + url + "\n应答：[" + json.result + "]" + json.msg)
            fun(json, ctx);
        }
    };
    xmlhttp.send();
}

*/