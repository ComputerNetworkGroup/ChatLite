## **client登录**
行为|maintype|subtype|
---|:--:|---:
请求登录|0x11(login)|0x00
登录成功（通知server）|0x11(login)|0x01
首次改密|0x11(login)|0x02


## **client向好友发送文本**

#### 报文头
行为|maintype|subtype
---|:--:|:---:|--
发送文本消息|0x14(sndTxt)|0x00


## **client向好友发送文件**
### 阶段1：发送消息头报文，得到文件id和分成多少个包

#### 报文头
行为|maintype|subtype
---|:--:|:---:|--
发送图片|0x12(sndFileHead)|0x01| 
发送jpg|0x12(sndFileHead)|0x02
发送gif|0x12(sndFileHead)|0x03

#### 报文内容
* int类型 id （代表文件编号）
* int类型 count （代表一共几个包）
* 最长32字节的char型 （文件名）

### 阶段2：按照数据包发送分包之后的数据

#### 报文头

行为|maintype|subtype
---|:--:|:---:|--
发送文件类型信息|0x13(sndFile)|0x00| 

#### 报文内容
* int类型 id （代表文件编号）
* int类型 count （代表第几个包）
* 最长为 MAXLENGTH 的数据


## **server回应登录**

行为|maintype|subtype|
---|:--:|---:
验证失败|0x71|0x00
验证成功|0x71|0x01
首次登录（强制改密）|0x71|0x02
设置密码错误|0x71|0x03
重复登录（强制下线）|0x71|0x04
重复登录（新上线）|0x71|0x05


## **server向client发送配置信息**

行为|maintype|subtype|
---|:--:|---:
窗口主题|0x81|0x01
好友列表|0x82|0x02

## **server回应转发状态**

#### 报文头
行为|maintype|subtype
---|:--:|:---:|--
成功转发|0x72|0x01
好友名称不存在|0x72|0xfe
好友未上线|0x72|0xfd






