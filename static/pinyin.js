function Reqdata(url, ctx, fun) {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            json = $.parseJSON(xmlhttp.responseText);
            if (json.result != "200")
                alert("������" + url+"\nӦ��["+json.result+"]"+json.msg)
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
                alert("������" + url + "\nӦ��[" + res.result + "]" + res.msg)
			if (cb != undefined)
				cb(res, ctx);
        }
    };
    xmlhttp.open("POST", url, true);
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    xmlhttp.send(jsn);
}

function Sendform(url, fd, ctx, cb) {
	alert("�����˲��Ͻ��ķ���Sendform()�������Ϊpostform");
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) { cb() }
    };

    xhr.open('POST', url, true);
    xhr.send(fd);
}

// ��������Sendform
// Sendform����������Ҫ�ⲿ��ǰ��fd����Ա�����Ӧ����Ϊ�˿ɿ���py��Ҳ��Ҫ��飩�������Ļ����Ĵ�����Ҫʵ�����Ρ�
// �˰汾����fd�ļ�飬��py��飬Ȼ��Ը�����Ӧ������д���
function postform(url, fd, ctx, cb) {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) { 
            res = $.parseJSON(xhr.responseText);
            if (res.result != "200")
                alert("������" + url + "\nӦ��[" + res.result + "]" + res.msg)
			if (cb != undefined)
				cb(res, ctx);
		}
    };

    xhr.open('POST', url, true);
    xhr.send(fd);
}


// ͬ����ʽ
function ReqdataS(url, ctx, fun) {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", url, false);
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            json = $.parseJSON(xmlhttp.responseText);
            if (json.result != "200")
                alert("������" + url + "\nӦ��[" + json.result + "]" + json.msg)
            fun(json, ctx);
        }
    };
    xmlhttp.send();
}

