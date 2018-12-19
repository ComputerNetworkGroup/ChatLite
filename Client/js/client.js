const MAXLENGTH = 2048;
const MAXNAMELEN = 32;
const MAXPASSWDLEN = 32;

const mt = {
    "login" : 0x11,
    "sndFileHead" : 0x12,
    "sndFile" : 0x13,
	"sndTxt" : 0X14,

	"resLogin" : 0x71,
	"resConf" : 0X81,
	"resSend" : 0X72,
};

const sbt = {
    "myDefault" : 0x00,

	"request" : 0x00,
	"success" : 0x01,
	"changepwd" : 0x02,

	"file" : 0x01,
	"jpg" : 0x02,
	"gif" : 0x03,

    "failed" : 0x00,
    "pwderror" : 0x03,
	"repeatout" : 0x04,
	"repeaton" : 0x05,

	"idNotExit" : 0xfe,
	"idOffline" : 0xfd,
};

function packetHeader() {
    this.mainType = new Buffer(1);
    this.subType = new Buffer(1);
    this.length = 0;
}

function Packet() {
    this.header = new packetHeader();
    this.msg = new Buffer(MAXLENGTH + 1024);

    this.isMainType = function(maintp) {
        return header.mainType == maintp;
    }

    this.isSubType = function(subtp) {
        return header.subType == subtp;
    }

    this.isType = function(maintp, subtp) {
        return (header.mainType == maintp) && (header.subType == subtp); 
    }

    this.getLen() = function() {
        return 0;
    }
}

function fileHeader() {
    this.friName = new Buffer(MAXNAMELEN);
    this.fileName = new Buffer(MAXNAMELEN);
    this.fileID = 0;
    this.packNum = 0;
}

function fileData() {
    this.friName = new Buffer(MAXNAMELEN);
    this.fileID = 0;
    this.count = 0;
    this.Buffer = new data(MAXLENGTH);
}

function loginData() {
    this.username = new Buffer(MAXNAMELEN);
    this.username = new Buffer(MAXPASSWDLEN);
}

function changePwdData() {
    this.newPasswd = new Buffer(MAXPASSWDLEN);
}

function fillPacketHeader(header, mainType, resType, msgLen){
    header.mainType = mainType;
    header.subType = resType;
    header.length = htons(msgLen+32); // TODO !
}

function socketSend(cfd, packet){
    var msgLen = getPacketLen(packet);
    var totalLen = 0;
    var sndLen;
    while(totalLen < msgLen){
        sndLen = send(cfd, &packet+totalLen, msgLen - totalLen, 0); // TODO !!!
        if(sndLen <= 0){
            alert("socket and msg error!\n");
            return 0;
        }
        totalLen += sndLen;
    }
    return 0;
}

function socketRecv(cfd, packet){
    var msgLen = 32;
    var totalLen = 0;
    var recvNum;

    while(totalLen < msgLen){
        recvNum = recv(cfd, packet.header + totalLen, msgLen-totalLen, 0); //TODO !!!!
        if(recvNum <= 0){
            alert("socket recv msg error!\n");
            return 0;
        }
        totalLen += recvNum;
    }

    msgLen = getPacketLen(packet);
    while(totalLen < msgLen){

    }
}

function roughSizeof( object ) {

    var objectList = [];
    var stack = [ object ];
    var bytes = 0;

    while ( stack.length ) {
        var value = stack.pop();

        if ( typeof value === 'boolean' ) {
            bytes += 4;
        }
        else if ( typeof value === 'string' ) {
            bytes += value.length * 2;
        }
        else if ( typeof value === 'number' ) {
            bytes += 8;
        }
        else if
        (
            typeof value === 'object'
            && objectList.indexOf( value ) === -1
        )
        {
            objectList.push( value );

            for( var i in value ) {
                stack.push( value[ i ] );
            }
        }
    }
    return bytes;
}

function sndLogin(cfd, username, passwd){
    var ulen = username.length;
    var plen = passwd.length;
    if(ulen > 32 || plen >> 32)
        return -1;
    
    var p = new Packet();
    fillPacketHeader(p.header, mt.login, sbt.request, roughSizeof(loginData)); // TODO

    var datap = p.msg   // TODO !!!

    username.copy(datap.username);
    passwd.copy(datap.passwd);

    return socketSend(cfd, p);
}

function clientRecv(cfd, packet){
    return socketRecv(cfd, packet);
}

function sndText(cfd, id, text){
    var p = new Packet();
    var idLen = idNum * 32;
    var textLen = text.length;

        // memcpy

    fillPacketHeader(p.header, mt.sndTxt, (unsigned char)idNum, idLen + textLen);   // TODO !!!!

    socketSend(cfd, p);

    return 0;
}

function sndJPG(cfd, id, jpgpath){
    return sndFileKind(cfd, id, sbt.jpg, jpgpath);
}

function sndGIF(cfd, id, gifpath) {
    return sndFileKind(cfd, id, sbt["gif"], gifpath);
}

function sndFile(cfd, id, filepath) {
    return sndFileKind (cfd, id, sbt["file"], filepath);
}

function firstChangePwd(cfd, newPwd) {
    var p = new Packet();
    var msgLen = newPwd.length;

    newPwd.copy(p.msg);

    fillPacketHeader(p.header, mt["login"], sbt["changepwd"], msgLen);

    return socketSent(cfd, p);

}

function sndResponse(cfd, maintype, subtype) {
    var p = new Packet();
    fillPackHeader(p.header, maintype, subtype, 0);

    return socketSend(cfd, p);
}

function alterPack(desPack, srcPack, srcId) {
    if (srcPack.isMainType(mt["sndFile"])) {
        alterTxtPack(desPack, secPack, srcId);
    }
    else if (srcPack.isMainType(mt["sndFileHead"])) {
        alterFileHeaderPack(desPack, srcPack, srcId);
    }
    else if (srcPack.isMainType(mt["sndFile"])) {
        alterFileDataPack(desPack, srcPack, srcId);
    }
    else {
        return -1;
    }
    
    return 0;
}


//创建tcp客户端
var net = require('net');

//IP地址
var HOST = '192.168.153.200';
//端口号
var PORT = 4000;

var client = new net.Socket();

//建立连接
function create_connection() {
    client.connect(PORT, HOSE, function() {
        console.log('Connected to: ' + HOST + ':' + PORT);
    });
}

//写msg
function write_msg(msg) {
    client.write(msg);
}

//当server端write时，会触发data事件
client.on('data', function(data) {
    console.log('DATA: ' + data);
})



client.on('close', function() {
    console.log('Connection closed');
});


// client.connect(PORT, HOST, function() {
//     console.log('CONNECTED TO: ' + HOST + ':' + PORT);
//     client.write('Hello!');
// });

// client.on('data', function(data) {
//     console.log('DATA: ' + data);
//     client.destroy();
// });

// client.on('close', function() {
//     console.log('Connection closed');
// });