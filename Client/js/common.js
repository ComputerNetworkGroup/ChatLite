const net = require('net');

const MAXLENGTH = 2048;
const MAXNAMELEN = 32;
const MAXPASSWDLEN = 32;

const cstate = {
    "online": '1',
    "offline": '0',
};

const mt = {
    "login": 0x11,
    "sndFileHead": 0x12,
    "sndFile": 0x13,
    "sndTxt": 0X14,

    "conf": 0x21,
    "updateList": 0x22,

    "resLogin": 0x71,
    "resConf": 0X81,
    "resSend": 0X72,
    "resFileHead": 0x73
};

const sbt = {
    "myDefault": 0x00,

    "request": 0x00,
    "success": 0x01,
    "changepwd": 0x02,

    "groupChat": 0x00,
    "file": 0x01,
    "jpg": 0x02,
    "gif": 0x03,

    "failed": 0x00,
    "pwderror": 0x03,
    "repeatout": 0x04,
    "repeaton": 0x05,
    "pwdChangeErr": 0x06,

    "idNotExit": 0xfe,
    "idOffline": 0xfd,

    "winTheme": 0x01,
    "friList": 0x02,
    "hisNum": 0x03,
    "fontcolor": 0x04,
    "fontsize": 0x05,
    "hisMessage": 0x06,

    "tellOnline": 0x01,
    "tellOffline": 0x02,
};

function Packet() {
    this.header = Buffer.alloc(4);
    this.msg = Buffer.alloc(MAXLENGTH + 1024);
}

function loginData() {
    this.username = Buffer.alloc(MAXNAMELEN);
    this.passwd = Buffer.alloc(MAXPASSWDLEN);
}

//socket发送
function socketSend(packet) {
    console.log(window.client.write(packet.header));
    console.log(packet.header.slice(2, 4).readInt16BE(0) - 4);
    console.log(window.client.write(packet.msg.slice(0, packet.header.slice(2, 4).readInt16BE(0) - 4)));
    console.log(window.client.bytesWritten);
    console.log(packet);
}

//填充报头
function fillPackHeader(packet, mainType, resType, msgLen) {
    packet.header[0] = mainType;
    packet.header[1] = resType;
    packet.header.slice(2, 4).writeInt16BE(msgLen + 4, 0);
}

//发送login消息
function sndLogin(username, passwd) {
    var ulen = username.length;
    var plen = passwd.length;
    if (ulen > 32 || plen > 32) {
        return -1;
    }

    var p = new Packet();

    fillPackHeader(p, mt["login"], sbt["request"], MAXNAMELEN + MAXPASSWDLEN);
    console.log(p);

    var datap = new loginData();

    datap.username.write(username);
    datap.passwd.write(passwd);
    console.log(datap);

    p.msg.slice(0, MAXNAMELEN).write(username)
    p.msg.slice(MAXNAMELEN, MAXNAMELEN + MAXPASSWDLEN).write(passwd);

    return socketSend(p);

}

//发送密码
function sndPwd(passwd) {
    var plen = passwd.length;
    if (plen > 32) {
        return -1;
    }

    var p = new Packet();

    fillPackHeader(p, mt["login"], sbt["changepwd"], MAXPASSWDLEN);
    console.log(p);

    p.msg.slice(0, MAXPASSWDLEN).write(passwd);

    return socketSend(p);
}

//发送连接成功消息
function sndSuccess() {
    var p = new Packet();

    fillPackHeader(p, mt["login"], sbt["success"], 0);

    return socketSend(p);
}

//发送聊天消息
function sndTxt(username, txt, num) {
    var p = new Packet();

    fillPackHeader(p, mt["sndTxt"], num, txt.length + 33);

    p.msg.slice(0, MAXNAMELEN).write(username);
    p.msg.slice(MAXNAMELEN).write(txt);

    return socketSend(p);
}

//发送查看历史消息
function sndHis(username) {
    var p = new Packet();

    fillPackHeader(p, mt["conf"], sbt["hisMessage"], MAXNAMELEN);

    p.msg.slice(0, MAXNAMELEN).write(username);

    return socketSend(p);
}

//发送配置信息
function sndConfig(hisnum_value, themecolor_r, themecolor_g, themecolor_b, fontsize_value, fontcolor_r, fontcolor_g, fontcolor_b) {
    var p1 = new Packet();
    fillPackHeader(p1, mt["conf"], sbt["hisNum"], 4);
    p1.msg.writeInt32LE(parseInt(hisnum_value));
    socketSend(p1);

    var p2 = new Packet();
    fillPackHeader(p2, mt["conf"], sbt["winTheme"], 4);
    p2.msg.writeInt8(parseInt(themecolor_r), 0);
    p2.msg.writeInt8(parseInt(themecolor_g), 1);
    p2.msg.writeInt8(parseInt(themecolor_b), 2);
    socketSend(p2);

    var p3 = new Packet();
    fillPackHeader(p3, mt["conf"], sbt["fontsize"], 4);
    p3.msg.writeUInt32LE(parseInt(fontsize_value));
    socketSend(p3);

    var p4 = new Packet();
    fillPackHeader(p4, mt["conf"], sbt["fontcolor"], 4);
    p4.msg.writeInt8(parseInt(fontcolor_r), 0);
    p4.msg.writeInt8(parseInt(fontcolor_r), 1);
    p4.msg.writeInt8(parseInt(fontcolor_r), 2);
    socketSend(p4);
}

//发送文件头
function sndFile_begin(username, filename, fileid, filecount) {
    var p = new Packet();

    fillPackHeader(p, mt["sndFileHead"], sbt["file"], 32+32+16+4);

    p.msg.slice(0, 32).write(username);
    p.msg.slice(32, 64).write(filename + '|');
    p.msg.slice(64, 80).write(fileid);
    p.msg.writeInt32LE(filecount, 80);

    return socketSend(p);
}

//发送文件
function sndFile_middle(username, fileid, fileNo, len, data) {
    var p = new Packet();

    fillPackHeader(p, mt["sndFile"], sbt["myDefault"], len+32+16+4);

    p.msg.slice(0, 32).write(username);
    p.msg.slice(32, 48).write(fileid);
    p.msg.writeInt32LE(fileNo, 48);
    p.msg.write(data, 52);

    return socketSend(p);
}

//发送确认接受文件
function accept_file(username, fileid) {
    var p = new Packet();

    fillPackHeader(p, mt["resFileHead"], sbt["success"], 48);

    p.msg.slice(0, 32).write(username);
    p.msg.slice(32, 48).write(fileid);

    return socketSend(p);
}

//发送拒绝接受文件
function reject_file(username, fileid) {
    var p = new Packet();

    fillPackHeader(p, mt["resFileHead"], sbt["failed"], 48);

    p.msg.slice(0, 32).write(username);
    p.msg.slice(32, 48).write(fileid);

    return socketSend(p);
}

function fileend(username) {
    var p = new Packet();

    fillPackHeader(p, mt["sndFile"], sbt["success"], 32);
    p.msg.slice(0, 32).write(username);

    return socketSend(p);
}

module.exports = {
    MAXLENGTH,
    MAXNAMELEN,
    MAXPASSWDLEN,
    cstate,
    mt,
    sbt,
    Packet,
    loginData,
    socketSend,
    sndLogin,
    sndPwd,
    sndSuccess,
    sndTxt,
    sndHis,
    sndConfig,
    sndFile_begin,
    sndFile_middle,
    accept_file,
    reject_file,
    fileend
}