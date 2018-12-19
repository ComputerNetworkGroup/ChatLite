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

function sndGIF(cfd, id, gifpath){
    return sndFileKind(cfd, id, sbt.gif, gifpath);
}

function sndFile(cfd, id, filepath){
    return sndFileKind(cfd, id, sbt.file, filepath);
}