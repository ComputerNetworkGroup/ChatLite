## **client��¼**
��Ϊ|maintype|subtype|
---|:--:|---:
�����¼|0x11(login)|0x00(request)
��¼�ɹ���֪ͨserver��|0x11(login)|0x01(success)
�״θ���|0x11(login)|0x02(changepwd)


## **client����ѷ����ı�**

#### ����ͷ
��Ϊ|maintype|subtype
---|:--:|:---:|--
�����ı���Ϣ|0x14(sndTxt)|0x00


## **client����ѷ����ļ�**
### �׶�1��������Ϣͷ���ģ��õ��ļ�id�ͷֳɶ��ٸ���

#### ����ͷ
��Ϊ|maintype|subtype
---|:--:|:---:|--
�����ļ�|0x12(sndFileHead)|0x01(file)
����jpg|0x12(sndFileHead)|0x02(jpg)
����gif|0x12(sndFileHead)|0x03(gif)

#### ��������
* 32�ֽڵ�char�� (���͸����ѵ��û���)
* 32�ֽڵ�char�� ���ļ�����
* int���� id �������ļ���ţ�
* int���� count ������һ����������

### �׶�2���������ݰ����ͷְ�֮�������

#### ����ͷ

��Ϊ|maintype|subtype
---|:--:|:---:|--
�����ļ�������Ϣ|0x13(sndFile)|0x00| 

#### ��������
* int���� id �������ļ���ţ�
* int���� count ������ڼ�������
* �Ϊ MAXLENGTH ������


## **server��Ӧ��¼**

��Ϊ|maintype|subtype|
---|:--:|---:
��֤ʧ��|0x71(resLogin)|0x00(failed)
��֤�ɹ�|0x71(resLogin)|0x01(success)
�״ε�¼��ǿ�Ƹ��ܣ�|0x71(resLogin)|0x02(changepwd)
�����������|0x71(resLogin)|0x03(pwderror)
�ظ���¼��ǿ�����ߣ�|0x71(resLogin)|0x04(repeatout)
�ظ���¼�������ߣ�|0x71(resLogin)|0x05(repeaton)


## **server��client����������Ϣ**

��Ϊ|maintype|subtype|
---|:--:|---:
��������|0x81(resConf)|0x01(winTheme)
�����б�|0x82(resConf)|0x02(friList)

## **server��Ӧת��״̬**

#### ����ͷ
��Ϊ|maintype|subtype
---|:--:|:---:|--
�ɹ�ת��|0x72(resSend)|0x01(success)
�������Ʋ�����|0x72(resSend)|0xfe(idNotExist)
����δ����|0x72(resSend)|0xfd(idOffline)






