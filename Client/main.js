'use strict';

const electron = require('electron');         //加载electron模块
const path = require('path');                 //加载path模块
const net = require('net');                   //加载net模块
const app = electron.app;                     //创建对electron应用对象的引用
const BrowserWindow = electron.BrowserWindow; //创建对electron的BrowserWindow类的引用


let mainWindow = null; //mainWindow变量保存了对应用视窗的引用

app.on('window-all-closed', () => {
    if(process.platform !== 'darwin') app.quit(); //监听所有视窗关闭的事件
});

app.on('ready', () => { //创建一个新的应用窗口并将它赋值给mainWindow变量，以此来防止Node.js进行垃圾回收的时候将视窗关闭
    mainWindow = new BrowserWindow();                    
    mainWindow.loadURL(`file://${__dirname}/html/base.html`);  //将index.html加载进应用视窗中
    mainWindow.on('closed', () => { mainWindow = null; }); //当应用关闭时，释放mainWindow变量对应用视窗的引用
})



var HOST = '192.168.153.200';  //IP地址
var PORT = 4000;               //端口号

var client = new net.Socket(); //创建socket的client端

client.connect(PORT, HOST, function() {
    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    client.write('Hello!');
});

client.on('data', function(data) {
    console.log('DATA: ' + data);
    client.destroy();
});

client.on('close', function() {
    console.log('Connection closed');
});