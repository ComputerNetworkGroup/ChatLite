## **client��¼**
��Ϊ|maintype|subtype|
---|:--:|---:
�����¼|0x11(login)|0x00(request)
��¼�ɹ���֪ͨserver��|0x11(login)|0x01(success)
�״θ���|0x11(login)|0x02(changepwd)


## **client����ѷ����ı�**

#### ����ͷ  

��Ϊ|maintype|subtype|
---|:--:|:---:
�����ı���Ϣ|0x14(sndTxt)| x(��������Ϣ�û�������)

#### ������Ϣ

* x �� 32�ֽڵ�char�� �����߻����ߵ��û��� name1 name2 name3 ...��
* �Ϊ MAXLENGTH ������

## **client���������޸ĺ����б���Ϣ**

#### ����ͷ  

��Ϊ|maintype|subtype|
---|:--:|:---:
֪ͨ����|0x22(updateList)| 0x01(tellOnline)
֪ͨ����|0x22(updateList)| 0x00(tellOffline)

#### ������Ϣ

* 32�ֽڵ�char�� �����߻����ߵ��û�����

## **client����������Ϣ**

#### ����ͷ  

��Ϊ|maintype|subtype|
---|:--:|:---:
��������|0x21(conf)| 0x01(winTheme)
������ʷ����|0x21(conf)| 0x03(hisNum)
����������ɫ|0x21(conf)| 0x04 (fontColor)
���������С|0x21(conf)| 0x05 (fontSize)
#### ������Ϣ

* 32�ֽڵ�char�� �����߻����ߵ��û�����




## **client����ѷ����ļ�**
### �׶�1��������Ϣͷ���ģ��õ��ļ�id�ͷֳɶ��ٸ�����������׶���ɴ��ļ�ָ�룬����id���ļ�ָ��Ĺ�ϵ��

#### ����ͷ  

��Ϊ|maintype|subtype|
---|:--:|:---:
�����ļ�|0x12(sndFileHead)|0x01(file)
����jpg|0x12(sndFileHead)|0x02(jpg)
����gif|0x12(sndFileHead)|0x03(gif)

#### ��������
* 32�ֽڵ�char�� ���û����� // ����Ϊ�����ߵ��û������հ�Ϊ�����ߵ��û�����server��������
* 32�ֽڵ�char�� ���ļ�����
* int���� id �������ļ���ţ�
* int���� count ������һ����������

### �׶�2���������ݰ����ͷְ�֮�������

#### ����ͷ  

��Ϊ|maintype|subtype|
---|:--:|:---:
�����ļ�������Ϣ|0x13(sndFile)|0x00(filedata)

#### ��������
* int���� id �������ļ���ţ� // ��ͨ���û����ж�
* int���� count ������ڼ�������
* �Ϊ MAXLENGTH ������

### �׶�3�������ļ����������־ ��֪ͨ�����ļ�������

#### ����ͷ  

��Ϊ|maintype|subtype|
---|:--:|:---:
�����ļ�������Ϣ|0x13(sndFile)|0x01(success)

#### ��������
* int���� id �������ļ���ţ�

## **server��Ӧ��¼**

��Ϊ|maintype|subtype|
---|:--:|:---:
��֤ʧ��|0x71(resLogin)|0x00(failed)
��֤�ɹ�|0x71(resLogin)|0x01(success)
�״ε�¼��ǿ�Ƹ��ܣ�|0x71(resLogin)|0x02(changepwd)
�������|0x71(resLogin)|0x03(pwderror)
�ظ���¼��ǿ�����ߣ�|0x71(resLogin)|0x04(repeatoff)
�ظ���¼�������ߣ�|0x71(resLogin)|0x05(repeaton)


## **server��client����������Ϣ**

��Ϊ|maintype|subtype|
---|:--:|:---:
��������|0x81(resConf)|0x01(winTheme)
�����б�|0x81(resConf)|0x02(friList)
��ʷ����|0x81(resConf)| 0x03(hisNum)


## **server��Ӧת��״̬**

#### ����ͷ  

��Ϊ|maintype|subtype|
---|:--:|:---:
�ɹ�ת��|0x72(resSend)|0x01(success)
�������Ʋ�����|0x72(resSend)|0xfe(idNotExit)
����δ����|0x72(resSend)|0xfd(idOffline)

#### ��������
* char name[32] ���������ƣ�





