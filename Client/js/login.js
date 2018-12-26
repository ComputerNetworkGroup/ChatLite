'use strict';

const net = require('net');
const common = require('../js/common');
const fs = require('fs');
const packjson = require("../package.json");

const chat_window = `
<div id="cover" style="position:absolute; width: 100%; height: 100%; left: 0; top: 0; background: rgba:(0, 0, 0, 0.5); display: none; z-index: 100"></div>
<div class="columns is-mobile" style="margin-top:3vh">
<div class="column is-three-fifths is-offset-one-fifth" style="background:white">
    <article class="media">
        <figure class="media-left">
            <nav class="panel">
                <p class="panel-heading">
                    <strong>Personl Information</strong>
                </p>
                <a class="panel-block" style="text-decoration:none" id="personal-info">
                    <span>Name: </span>
                </a>
                <!--<a class="panel-block" style="text-decoration:none">-->
                    <a class="button" href="javascript:void(0)" onclick="per_set()" id="set1" style="margin-top:5px; margin-left:10px">Personal Set</a>
                    <a class="button" href="javascript:void(0)" onclick="get_meshis()" id="set2" style="margin-top:5px; margin-left:15px">Chat Record</a>
                    <!--<input class="button" type="button" name="set1" value="Personal Set" id="set1" style="margin-right:3px">
                    <input class="button" type="button" name="set2" value="Chat Record" id="set2" style="margin-left:3px">-->
                <!--</a>-->
            </nav>
            <nav class="panel">
                <p class="panel-heading">
                    <i class="fas fa-users"></i>
                    <strong>Chat Group User</strong>
                </p>
                <div style="width:33vh; height:48vh; overflow-y:auto">
                    <aside class="menu">
                        <ul class="menu-list">
                            <li><a href="javascript:void(0)" onclick="group_transfer()" id="group-chat"><i class="fas fa-users"></i>&nbspGroup Chat</a></li>
                            <li>
                                <a><i class="fas fa-user"></i>&nbspOnline User</a>
                                <ul id="online-user">
                                </ul>
                            </li>
                            <li>
                                <a><i class="fas fa-user"></i>&nbspOffline User</a>
                                <ul id="offline-user">
                                </ul>
                            </li>
                        </ul>
                    </aside>
                </div>
            </nav>
        </figure>
        <div class="media-content">
            <div class="field">
                <nav class="panel">
                    <p class="panel-heading" id="user-chat-msg"><strong>Chat Content</strong></p>
                    <div class="panel-block">
                        <div id="chat-box" style="position:relative; height:50vh; overflow-y:auto; font-size:16px">
                        </div>
                    </div>
                </nav>
                <!-- <div class="message">
                                <textarea class="textarea" placeholder="" rows="15" readonly></textarea>
                            </div> -->
            </div>
            <div class="field">
                <p class="control">
                    <textarea class="textarea" placeholder="Please enter your chat content..." id="txt"></textarea>
                </p>
            </div>
            <nav class="level">
                <div class="level-left">
                    <!-- <div class="level-item">
                                    <a class="button is-info">Submit</a>
                                </div> -->
                </div>
                <div class="level-right">
                    <div class="file" style="margin-right:5px">
                        <label class="file-label">
                            <input class="file-input" type="file" name="resume" id="file" onchange="getfilename()">
                            <span class="file-cta">
                                <span class="file-icon">
                                    <i class="fas fa-upload"></i>
                                </span>
                                <span class="file-label" id="filename">
                                    Choose a file…
                                </span>
                            </span>
                        </label>
                    </div>
                    <div class="level-item">
                        <a class="button is-info" href="javascript:void(0)" onclick="msg_submit()">Submit</a>
                    </div>
                </div>
            </nav>
        </div>
    </article>
</div>
</div>
<script>
function getfilename() {
    var file = document.getElementById("file");
    if (file.files.length > 0) {

        console.log(file.files);

        document.getElementById('filename').innerHTML = file.files[0].name;

    }
}
</script>
`


const HOST = packjson.HOST;  //IP地址
const PORT = packjson.PORT;  //端口号

var account_value;
var password_value;

window.dic = new Array();

//创建新的socket
window.client = new net.Socket();

//2333
// var writeStream;
var filename233;
var filecount1;



// //建立连接
// window.client.connect(PORT, HOST, function () {
//     console.log('CONNECTED TO: ' + HOST + ':' + PORT);
// })

window.client.on('error', function (err) {
    console.log(err);
})

window.BufLength = 0;
window.Buf = new Buffer.alloc(100000);

function compare_twostr(str1, str2) {
    for (let i = 0; i < str1.length; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

//监听server端的write
window.client.on('data', function (data) {
    // console.log('DATA: ' + data);
    console.log(data.length);
    data.copy(Buf, BufLength);
    // Buf.slice(BufLength, data.length + BufLength) = data;
    BufLength = BufLength + data.length;
    var p = new common.Packet();
    var plen = 4;
    while (1) {
        console.log('BufLength:' + BufLength);
        if (BufLength >= 4) {
            Buf.copy(p.header, 0, 0, 4);
            // p.header[0] = Buf[0];
            // p.header[1] = Buf[1];
            // p.header[2] = Buf[2];
            // p.header[3] = Buf[3];
            plen = p.header.slice(2, 4).readInt16BE(0);
            console.log('plen:' + plen);
        }
        else {
            break;
        }
        if (BufLength >= plen) {
            Buf.copy(p.msg, 0, 4, plen);
            // p.msg.slice(0, plen - 4) = Buf.slice(4, plen);
            Buf.copy(Buf, 0, p.header.slice(2, 4).readInt16BE(0), BufLength);
            BufLength = BufLength - plen;
            console.log("*packet*");
            console.log(p);

            if (p.header[0] == common.mt["resLogin"]) {
                //回应登录
                if (p.header[1] == common.sbt["failed"]) {
                    //验证失败
                    $('#error-msg').empty();
                    $('#error-msg').append('<span style="color:red">This User does not Exist</span>')
                }
                else if (p.header[1] == common.sbt["success"]) {
                    //验证成功
                    common.sndSuccess();
                    document.getElementById('login-card').classList.remove('is-active');
                    document.getElementById('change-pwd').classList.remove('is-active');
                    $('#hello-msg').append('<span>Hello,&nbsp%data%!</span>'.replace('%data%', account_value));
                    $('#narbar-button').empty();
                    $('#narbar-button').append('<a class="button is-light" id="logout" href="javascript:void(0)" onclick="Log_Out()">Log Out</a > ');
                    $('#welcome').remove();
                    $('body').append(chat_window);
                    $('#personal-info').append('<span>&nbsp%data%</span>'.replace('%data%', account_value));
                }
                else if (p.header[1] == common.sbt["changepwd"]) {
                    //首次登录,进入改密
                    document.getElementById('login-card').classList.remove('is-active');
                    document.getElementById('change-pwd').classList.add('is-active');
                }
                else if (p.header[1] == common.sbt["pwderror"]) {
                    //密码错误
                    $('#error-msg').empty();
                    $('#error-msg').append('<span style="color:red">Password Error!</span>')
                }
                else if (p.header[1] == common.sbt["pwdChangeErr"]) {
                    $('#error-msg1').empty();
                    $('#error-msg1').append('<span style="color:red">New Password and Old Password are the Same!</span>');
                }
                else if (p.header[1] == common.sbt["repeaton"]) {
                    alert("Repeat On!");
                    common.sndSuccess();
                    document.getElementById('login-card').classList.remove('is-active');
                    document.getElementById('change-pwd').classList.remove('is-active');
                    $('#hello-msg').append('<span>Hello,&nbsp%data%!</span>'.replace('%data%', account_value));
                    $('#narbar-button').empty();
                    $('#narbar-button').append('<a class="button is-light" id="logout" href="javascript:void(0)" onclick="Log_Out()">Log Out</a > ');
                    $('#welcome').remove();
                    $('body').append(chat_window);
                    $('#personal-info').append('<span>&nbsp%data%</span>'.replace('%data%', account_value));
                }
                else if (p.header[1] == common.sbt["repeatout"]) {
                    alert("Repeat Out!");
                }
            }
            else if (p.header[0] == common.mt["resConf"]) {
                //修改配置
                if (p.header[1] == common.sbt["winTheme"]) {
                    //修改背景色
                    var r = p.msg[0].toString();
                    console.log(r);
                    var g = p.msg[1].toString();
                    console.log(g);
                    var b = p.msg[2].toString();
                    console.log(b);
                    $('body').css('background', 'linear-gradient(180deg, rgb(' + r + ',' + g + ',' + b + '), white)');
                    $('body').css('height', '100vh');
                }
                if (p.header[1] == common.sbt["fontsize"]) {
                    //改字体
                    var font_size = p.msg.readInt32LE(0).toString + 'px';
                    $('#chat-box').css('font-size', font_size);

                }
                if (p.header[1] == common.sbt["fontcolor"]) {
                    var r = p.msg[0].toString();
                    console.log(r);
                    var g = p.msg[1].toString();
                    console.log(g);
                    var b = p.msg[2].toString();
                    console.log(b);
                    $('#chat-box').css('color', 'rgb(' + r + ',' + g + ',' + b + ')');

                }
                if (p.header[1] == common.sbt["friList"]) {
                    //获取用户列表
                    var userlist = p.msg.toString().split('|');
                    console.log(userlist);
                    window.dic['GroupChat'] = new Array();
                    for (let i = 0; i < userlist.length - 1; i++) {
                        var username = userlist[i].slice(1, userlist[i].length);
                        window.dic[username] = new Array();
                        console.log(userlist[i]);
                        if (userlist[i][0] == '1') {
                            //用户i在线
                            $('#online-user').append(`<li id='%data%'><a href='javascript:void(0)' onclick='chat_transfer("%data%")'><i class='fas fa-user'></i>&nbsp%data%</a></li>`.replace('%data%', username).replace('%data%', username).replace('%data%', username));
                        }
                        else {
                            //用户i不在线
                            $('#offline-user').append(`<li id='%data%'><a href='javascript:void(0)' onclick='chat_transfer("%data%")'><i class='fas fa-user'></i>&nbsp%data%</a></li>`.replace('%data%', username).replace('%data%', username).replace('%data%', username));
                        }
                    }
                }
                if (p.header[1] == common.sbt["hisMessage"]) {
                    //获取历史消息
                    var messagelist = p.msg.toString().split('|');
                    console.log(messagelist);
                    for (let i = 0; i < messagelist.length - 1; i++) {
                        var mes = messagelist[i].slice(1);
                        if (messagelist[i][0] == '1') {
                            //自己发的
                            $('#chat-box').append(`<div style="text-align: right; margin-left: 5vw; width: 380px">
                               <span style="font-size:10px; color:green">` + account_value + ` ` + `history` + `</span>
                               <br>
                               <span>` + mes + `</span>
                               </div>`);
                        }
                        else {
                            //对方发的
                            $('#chat-box').append(`<div style="text-align: left; margin-right: 5vw; width: 380px">
                               <span style="font-size:10px; color:blue">` + $('p#user-chat-msg strong').text() + ` ` + `history` + `</span>
                               <br>
                               <span>` + mes + `</span>
                               </div>`);
                        }
                    }
                }
            }
            else if (p.header[0] == common.mt["updateList"]) {
                //有用户上/下线
                var username = p.msg.toString('utf8', 0, plen - 5);
                console.log(username);
                if (p.header[1] == common.sbt["tellOnline"]) {
                    $('#%data%'.replace('%data%', username)).remove();
                    $('#online-user').append(`<li id='%data%'><a href='javascript:void(0)' onclick='chat_transfer("%data%")'><i class='fas fa-user'></i>&nbsp%data%</a></li>`.replace('%data%', username).replace('%data%', username).replace('%data%', username));
                }
                else {
                    $('#%data%'.replace('%data%', username)).remove();
                    $('#offline-user').append(`<li id='%data%'><a href='javascript:void(0)' onclick='chat_transfer("%data%")'><i class='fas fa-user'></i>&nbsp%data%</a></li>`.replace('%data%', username).replace('%data%', username).replace('%data%', username));
                }
            }
            else if (p.header[0] == common.mt["sndTxt"]) {
                //收到文本信息
                if (p.header[1] == 0x00) {
                    //群发
                    var username = p.msg.slice(0, common.MAXNAMELEN).toString().split('|')[0];
                    var txt = p.msg.slice(common.MAXNAMELEN, plen - 4);
                    var myDate = new Date();
                    dic['GroupChat'].push(username);
                    dic['GroupChat'].push(txt);
                    dic['GroupChat'].push(myDate.toLocaleTimeString());
                    if ($('p#user-chat-msg strong').text() == 'GroupChat') {
                        $('#chat-box').append(`<div style="text-align: left; margin-right: 5vw; width: 380px">
                                               <span style="font-size:10px; color:blue">` + username + ` ` + myDate.toLocaleTimeString() + `</span>
                                               <br>
                                               <span>` + txt + `</span>
                                               </div>`);
                    }
                    else {
                        $('#group-chat').append('&nbsp<i class="fas fa-bell"></i>');
                    }
                }
                else {
                    //单发
                    var username = p.msg.slice(0, common.MAXNAMELEN).toString().split('|')[0];
                    var txt = p.msg.slice(common.MAXNAMELEN, plen - 4);
                    // console.log("JHL SHIT1");
                    // console.log($('p#user-chat-msg strong').text());
                    // console.log(typeof($('p#user-chat-msg strong').text()));
                    // console.log(username);
                    // console.log(typeof(username));
                    // console.log(username.length);
                    // console.log($('p#user-chat-msg strong').text() === username);
                    var myDate = new Date();
                    dic[username].push('0' + txt);
                    dic[username].push(myDate.toLocaleTimeString());
                    // if (compare_twostr($('p#user-chat-msg strong').text(), username)) {
                    if ($('p#user-chat-msg strong').text() == username) {
                        //当前聊天界面是username
                        console.log("JHL SHIT2");

                        $('#chat-box').append(`<div style="text-align: left; margin-right: 5vw; width: 380px">
                               <span style="font-size:10px; color:blue">` + $('p#user-chat-msg strong').text() + ` ` + myDate.toLocaleTimeString() + `</span>
                               <br>
                               <span>` + txt + `</span>
                               </div>`);
                    }
                    else {
                        //当前聊天界面不是username
                        $('#%data%'.replace('%data%', username)).remove();
                        $('#online-user').prepend(`<li id='%data%'><a href='javascript:void(0)' onclick='chat_transfer("%data%")'><i class='fas fa-user'></i>&nbsp%data%&nbsp<i class="fas fa-bell"></i></a></li>`.replace('%data%', username).replace('%data%', username).replace('%data%', username));
                    }
                }
            }
            else if (p.header[0] == common.mt["sndFileHead"]) {
                //对方要发送文件的开始标记
                if (p.header[1] == common.sbt["file"]) {
                    var username = p.msg.slice(0, 32).toString().split('|')[0];
                    var filename = p.msg.slice(32, 64).toString().split('|')[0];

                    //2333
                    // writeStream = fs.createWriteStream(filename);
                    filename233 = filename;

                    // writeStream.setDefaultEncoding('utf8');
                    var fileid = p.msg.slice(64, 80).toString();//todo
                    filecount1 = p.msg.readInt32LE(80);
                    console.log("!filecount! " + filecount1);

                    document.getElementById('sndFile').classList.add('is-active');

                    $('#sndfile-msg').empty();
                    $('#sndfile-msg').append('<span>User %data% want to send a file called %data1% to you!</span>'.replace('%data%', username).replace('%data1%', filename));
                    $('#sndfile-msg').append('<span id="file-id" style="display:none">%data%</span>'.replace('%data%', fileid));
                    $('#sndfile-msg').append('<span id="user-name" style="display:none">%data%</span>'.replace('%data%', username));
                }
            }
            else if (p.header[0] == common.mt["resFileHead"]) {
                //发送文件回复
                if (p.header[1] == common.sbt["failed"]) {
                    //对方拒绝
                    alert("The other party rejected your request to send");
                }
                else {
                    //对方接受

                    var file = document.getElementById("file");

                    document.getElementById('sndFilePro').classList.add('is-active');

                    //传文件
                    var readText = fs.readFileSync(file.files[0].path);
                    console.log(typeof (readText));
                    var fileSize = readText.length;
                    const filePacketSize = 1024 * 2;
                    var fileCount = Math.ceil(fileSize / filePacketSize);

                    const readStream = fs.createReadStream(file.files[0].path, { highWaterMark: 2 * 1024 });
                    // readStream.setDefaultEncoding('utf8');
                    var ctr = 0;

                    readStream.on('data', function (data) {
                        console.log(data.length);
                        // console.log("!!!data:" + data);
                        console.log("ctr:" + ctr);
                        common.sndFile_middle($('p#user-chat-msg strong').text(), '1', ctr, data.length, String(data));
                        ctr++;
                        $('#span1').text(String(Math.floor(ctr / fileCount * 100)) + '%');
                        $('#process1').text(String(Math.floor(ctr / fileCount * 100)) + '%');
                        $('#process1').val(String(Math.floor(ctr / fileCount * 100)));
                        if (data.length < 2048) {
                            common.fileend($('p#user-chat-msg strong').text());
                        }
                    });
                    document.getElementById('sndFilePro').classList.remove('is-active');
                    document.getElementById('filename').innerHTML = "Choose one file...";
                    document.getElementById('file').value = '';
                    document.getElementById('file').outerHTML = document.getElementById('file').outerHTML;
                }
            }
            else if (p.header[0] == common.mt["sndFile"]) {
                if (p.header[1] == common.sbt["success"]) {
                    //2333
                    // writeStream.end();
                    document.getElementById('rcvFilePro').classList.remove('is-active');
                }
                else {
                    // console.log("***" + p.msg.slice(52).toString());
                    console.log("###" + plen);
                    //2333
                    // writeStream.write(p.msg.slice(52, plen - 4));

                    var ctr = p.msg.readInt32LE(48);
                    console.log("!ctr! " + ctr);
                    
                    $('#span2').text(String(Math.floor(ctr / filecount1 * 100)) + '%');
                    $('#process2').text(String(Math.floor(ctr / filecount1 * 100)) + '%');
                    $('#process2').val(String(Math.floor(ctr / filecount1 * 100)));

                    
                    fs.writeFileSync(filename233, p.msg.slice(52, plen - 4).toString(), {
                        flag: 'a'
                    });
                }

            }
        }
        else {
            break;
        }
    }
});

//监听login按钮，显示login card
document.getElementById('login').addEventListener('click', function () {
    $('#error-msg').empty();
    document.getElementById('login-card').classList.add('is-active');
})

//监听login-submit按钮
document.getElementById('submit-btn').addEventListener('click', function () {
    account_value = document.getElementById('account').value;
    password_value = document.getElementById('password').value;
    console.log(account_value, password_value);
    document.getElementById('account').value = '';
    document.getElementById('password').value = '';

    //判断是否输入account和password
    if (account_value.length > 0 && password_value.length > 0) {
        //如果accont和password都已输入
        //给server发包，判断该账户是否存在等
        //建立连接
        window.client.connect(PORT, HOST, function () {
            console.log('CONNECTED TO: ' + HOST + ':' + PORT);
        });
        common.sndLogin(account_value, password_value);
    }
    else {
        //如果account和password未输入
        $('#error-msg').empty();
        $('#error-msg').append('<span style="color:red">Please Enter Your Account or Password!</span>')
    }
})

document.getElementById('close-btn').addEventListener('click', function () {
    document.getElementById('login-card').classList.remove('is-active');
})

document.getElementById('close-btn1').addEventListener('click', function () {
    document.getElementById('config-card').classList.remove('is-active');
})

window.client.on('close', function () {
    $('#cover').css('display', 'block');
    alert("You are offline!");
    console.log('Connection Closed!');
})

//监听change-pwd-btn按钮
var change_pwd_btn = document.getElementById('change-pwd-btn');
change_pwd_btn.addEventListener('click', function () {

    var np1_value = document.getElementById('np1').value;
    var np2_value = document.getElementById('np2').value;
    console.log(np1_value);
    console.log(np2_value);
    document.getElementById('np1').value = '';
    document.getElementById('np2').value = '';
    //判断是否输入
    if (np1_value.length > 0 && np2_value.length > 0) {
        //如果有输入
        //判断前后两次密码是否一致
        if (np1_value != np2_value) {
            //如果不一致
            $('#error-msg1').empty();
            $('#error-msg1').append('<span style="color:red">Two Passwords are Inconsistent!</span>');
        }
        else {
            //如果前后两次密码一致
            common.sndPwd(np1_value);
            // if (true) {
            //     $('#error-msg1').empty();
            //     $('#error-msg1').append('<span style="color:red">New Password and Old Password are the Same!</span>');
            // }
            // else {

            // }
        }
    }
    else {
        $('#error-msg1').empty();
        $('#error-msg1').append('<span style="color:red">Please Enter Your New Password and Repeat it!</span>');
    }
});

//切换聊天页面
function chat_transfer(username) {
    console.log(username);
    console.log(typeof (username));
    $('li#' + username + ' a').addClass('is-active');
    if ($('p#user-chat-msg strong').text() != username) {
        $('li#' + username + ' svg.fa-bell').remove();
        $('#group-chat').removeClass('is-active');
        $('li#' + $('p#user-chat-msg strong').text() + ' a').removeClass('is-active');
        $('#user-chat-msg').empty();
        $('#user-chat-msg').append('<strong>%data%</strong>'.replace('%data%', username));
        $('#chat-box').empty();
        //添加历史消息
        for (let i = 0; i < dic[username].length; i = i + 2) {
            var txt = dic[username][i].slice(1);
            if (dic[username][i][0] == '1') {
                $('#chat-box').append(`<div style="text-align: right; margin-left: 5vw; width: 380px">
                               <span style="font-size:10px; color:green">` + account_value + ` ` + dic[username][i + 1] + `</span>
                               <br>
                               <span>` + txt + `</span>
                               </div>`);
            }
            else {
                $('#chat-box').append(`<div style="text-align: left; margin-right: 5vw; width: 380px">
                               <span style="font-size:10px; color:blue">` + username + ` ` + dic[username][i + 1] + `</span>
                               <br>
                               <span>` + txt + `</span>
                               </div>`);
            }
        }
    }
}


//群聊页面切换
// {/* <li><a href="javascript:void(0)" onclick="group_transfer()" id="group-chat"><i class="fas fa-users"></i>&nbspGroup Chat</a></li> */}
function group_transfer() {
    $('#group-chat').addClass('is-active');
    $('#user-chat-msg').empty();
    $('#user-chat-msg').append('<strong>%data%</strong>'.replace('%data%', 'GroupChat'));
    $('#chat-box').empty();
    $('#group-chat svg.fa-bell').remove();
    //添加历史消息
    for (let i = 0; i < dic['GroupChat'].length; i = i + 3) {
        var username = dic['GroupChat'][i];
        var txt = dic['GroupChat'][i + 1];
        if (username == account_value) {
            $('#chat-box').append(`<div style="text-align: right; margin-left: 5vw; width: 380px">
                           <span style="font-size:10px; color:green">` + account_value + ` ` + dic['GroupChat'][i + 2] + `</span>
                           <br>
                           <span>` + txt + `</span>
                           </div>`);
        }
        else {
            $('#chat-box').append(`<div style="text-align: left; margin-right: 5vw; width: 380px">
                           <span style="font-size:10px; color:blue">` + username + ` ` + dic['GroupChat'][i + 2] + `</span>
                           <br>
                           <span>` + txt + `</span>
                           </div>`);
        }
    }
}

//发送消息submit按钮点击函数
function msg_submit() {
    var txt = document.getElementById('txt').value;
    console.log('txt:' + txt);
    document.getElementById('txt').value = '';

    var file = document.getElementById("file");
    if (file.files.length > 0) {
        //传文件
        var readText = fs.readFileSync(file.files[0].path);
        console.log(typeof (readText));
        var fileSize = readText.length;
        const filePacketSize = 1024 * 2;
        var fileCount = Math.ceil(fileSize / filePacketSize);

        common.sndFile_begin($('p#user-chat-msg strong').text(), file.files[0].name, '0', fileCount);
        // document.getElementById('filename').innerHTML = "Choose one file...";
    }
    else {
        //判断用户是否发送空消息
        if (txt.length > 0) {
            //不是空消息
            var myDate = new Date();
            if ($('p#user-chat-msg strong').text() == 'GroupChat') {
                dic['GroupChat'].push(account_value);
                dic['GroupChat'].push(txt);
                dic['GroupChat'].push(myDate.toLocaleTimeString());
                common.sndTxt(account_value, txt, 0);
            }
            else {
                dic[$('p#user-chat-msg strong').text()].push('1' + txt);
                dic[$('p#user-chat-msg strong').text()].push(myDate.toLocaleTimeString());
                common.sndTxt($('p#user-chat-msg strong').text(), txt, 1);
            }
            $('#chat-box').append(`<div style="text-align: right; margin-left: 5vw; width: 380px">
                           <span style="font-size:10px; color:green">` + account_value + ` ` + myDate.toLocaleTimeString() + `</span>
                           <br>
                           <span>` + txt + `</span>
                           </div>`);
        }
        else {
            alert('Cannot send null message!');
        }
    }

}

//监听修改个人配置按钮
function per_set() {
    $('#error-msg2').empty();
    document.getElementById('config-card').classList.add('is-active');
}

//监听查看历史消息按钮
function get_meshis() {
    var username = $('p#user-chat-msg strong').text();
    common.sndHis(username);
    // window.dic[username] = new Array();
    $('#chat-box').empty();
}

//监听文件接受确认键
document.getElementById('accept-btn').addEventListener('click', function () {
    var fileid = $('#file-id').text();
    var username = $('#user-name').text();
    common.accept_file(username, fileid);
    document.getElementById('sndFile').classList.remove('is-active');
    document.getElementById('rcvFilePro').classList.add('is-active');
})


//监听文件接受拒绝键
document.getElementById('reject-btn').addEventListener('click', function () {
    var fileid = $('#file-id').text();
    var username = $('#user-name').text();
    common.reject_file(username, fileid);
    document.getElementById('sndFile').classList.remove('is-active');
})



//监听config-submit按钮
document.getElementById('config-btn').addEventListener('click', function () {
    var hisnum_value = document.getElementById('his-num').value;
    var themecolor_r = document.getElementById('theme-color-r').value;
    var themecolor_g = document.getElementById('theme-color-g').value;
    var themecolor_b = document.getElementById('theme-color-b').value;
    var fontsize = document.getElementById('fontsize');
    var fontsize_value = fontsize.options[fontsize.selectedIndex].text.slice(0, 2);
    var fontcolor_r = document.getElementById('fontcolor-r').value;
    var fontcolor_g = document.getElementById('fontcolor-g').value;
    var fontcolor_b = document.getElementById('fontcolor-b').value;
    // console.log(hisnum_value, themecolor_value, fontsize_value, fontcolor_value);
    document.getElementById('his-num').value = '';
    document.getElementById('theme-color-r').value = '';
    document.getElementById('theme-color-g').value = '';
    document.getElementById('theme-color-b').value = '';
    document.getElementById('fontcolor-r').value = '';
    document.getElementById('fontcolor-g').value = '';
    document.getElementById('fontcolor-b').value = '';

    //判断是否输入
    if (hisnum_value.length > 0 && themecolor_r.length > 0 && themecolor_g.length > 0 && themecolor_b.length > 0 && fontsize_value.length > 0 && fontcolor_r.length > 0 && fontcolor_g.length > 0 && fontcolor_b.length > 0) {
        //如果都已输入
        //给server发包，判断该账户是否存在等
        // common.sndLogin(account_value, password_value);
        $('body').css('background', 'linear-gradient(180deg, rgb(' + themecolor_r + ',' + themecolor_g + ',' + themecolor_b + '), white)');
        $('body').css('height', '100vh');

        $('#chat-box').css('font-size', fontsize_value + 'px');

        $('#chat-box').css('color', 'rgb(' + fontcolor_r + ',' + fontcolor_g + ',' + fontcolor_b + ')');

        document.getElementById('config-card').classList.remove('is-active');

        common.sndConfig(hisnum_value, themecolor_r, themecolor_g, themecolor_b, fontsize_value, fontcolor_r, fontcolor_g, fontcolor_b);
    }
    else {
        //如果未输入
        $('#error-msg2').empty();
        $('#error-msg2').append('<span style="color:red">Please Enter Your Personal Set!</span>');
    }
})

function Log_Out() {
    $('#cover').css('display', 'block');
    window.client.destroy();
}
// document.getElementById('logout').addEventListener('click', function () {
//     $('#cover').css('display', 'block');
// })

// var file = document.getElementById("file");
// file.onchange = function () {
//     if (file.files.length > 0) {

//         document.getElementById('filename').innerHTML = file.files[0].name;

//     }
// };


// document.getElementById('set2').addEventListener('click', function () {
//     var username = $('p#user-chat-msg strong').text();
//     common.sndHis(username);
//     // window.dic[username] = new Array();
//     $('#chat-box').empty();
// })

// //监听发送消息submit按钮
// document.getElementById('sndtxt-btn').addEventListener('click', function () {
//     var txt = document.getElementById('txt').value;
//     document.getElementById('txt').value = '';
//     //判断用户是否发送空消息
//     if (txt.length > 0) {
//         //不是空消息
//         common.sndTxt($('p#user-chat-msg strong').txt(), txt)
//     }
//     else {
//         alert('Cannot send null message!');
//     }
// });