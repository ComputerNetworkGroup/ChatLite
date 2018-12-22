## **client登录**
行为|maintype|subtype|
---|:--:|---:
请求登录|0x11(login)|0x00(request)
登录成功（通知server）|0x11(login)|0x01(success)
首次改密|0x11(login)|0x02(changepwd)


## **client向好友发送文本**

#### 报文头  

行为|maintype|subtype|
---|:--:|:---:
发送文本消息|0x14(sndTxt)| x(代表发送信息用户名数量)

#### 报文信息

* x 个 32字节的char型 （上线或下线的用户名 name1 name2 name3 ...）
* 最长为 MAXLENGTH 的数据

## **client上线下线修改好友列表信息**

#### 报文头  

行为|maintype|subtype|
---|:--:|:---:
通知上线|0x22(updateList)| 0x01(tellOnline)
通知下线|0x22(updateList)| 0x00(tellOffline)

#### 报文信息

* 32字节的char型 （上线或下线的用户名）

## **client更改配置信息**

#### 报文头  

行为|maintype|subtype|
---|:--:|:---:
更改主题|0x21(conf)| 0x01(winTheme)
更改历史长度|0x21(conf)| 0x03(hisNum)
更改字体颜色|0x21(conf)| 0x04 (fontColor)
更改字体大小|0x21(conf)| 0x05 (fontSize)
#### 报文信息

* 32字节的char型 （上线或下线的用户名）




## **client向好友发送文件**
### 阶段1：发送消息头报文，得到文件id和分成多少个包（在这个阶段完成打开文件指针，建立id到文件指针的关系）

#### 报文头  

行为|maintype|subtype|
---|:--:|:---:
发送文件|0x12(sndFileHead)|0x01(file)
发送jpg|0x12(sndFileHead)|0x02(jpg)
发送gif|0x12(sndFileHead)|0x03(gif)

#### 报文内容
* 32字节的char型 （用户名） // 发包为接收者的用户名，收包为发送者的用户名，server端做处理
* 32字节的char型 （文件名）
* int类型 id （代表文件编号）
* int类型 count （代表一共几个包）

### 阶段2：按照数据包发送分包之后的数据

#### 报文头  

行为|maintype|subtype|
---|:--:|:---:
发送文件类型信息|0x13(sndFile)|0x00(filedata)

#### 报文内容
* int类型 id （代表文件编号） // 仅通过用户名判断
* int类型 count （代表第几个包）
* 最长为 MAXLENGTH 的数据

### 阶段3：发送文件传输结束标志 （通知传输文件结束）

#### 报文头  

行为|maintype|subtype|
---|:--:|:---:
发送文件类型信息|0x13(sndFile)|0x01(success)

#### 报文内容
* int类型 id （代表文件编号）

## **server回应登录**

行为|maintype|subtype|
---|:--:|:---:
验证失败|0x71(resLogin)|0x00(failed)
验证成功|0x71(resLogin)|0x01(success)
首次登录（强制改密）|0x71(resLogin)|0x02(changepwd)
密码错误|0x71(resLogin)|0x03(pwderror)
重复登录（强制下线）|0x71(resLogin)|0x04(repeatoff)
重复登录（新上线）|0x71(resLogin)|0x05(repeaton)


## **server向client发送配置信息**

行为|maintype|subtype|
---|:--:|:---:
窗口主题|0x81(resConf)|0x01(winTheme)
好友列表|0x81(resConf)|0x02(friList)
历史长度|0x81(resConf)| 0x03(hisNum)


## **server回应转发状态**

#### 报文头  

行为|maintype|subtype|
---|:--:|:---:
成功转发|0x72(resSend)|0x01(success)
好友名称不存在|0x72(resSend)|0xfe(idNotExit)
好友未上线|0x72(resSend)|0xfd(idOffline)

#### 报文内容
* char name[32] （好友名称）





