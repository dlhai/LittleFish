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

