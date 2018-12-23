DROP DATABASE IF EXISTS db1651025; 
CREATE DATABASE db1651025;
use db1651025;

CREATE TABLE userlist(
id VARCHAR(32) PRIMARY KEY,
passwd VARCHAR(32),
log_time DATETIME 
);

CREATE TABLE settings(
id VARCHAR(32) PRIMARY KEY,
set_fontcolor INTEGER,
set_color INTEGER,
set_msgnum INTEGER,
set_fontsize INTEGER,
FOREIGN KEY (id) REFERENCES userlist(id)
);


CREATE TABLE msgs(
from_id VARCHAR(32),
to_id VARCHAR(32),
msg_time DATETIME ,
msg VARCHAR(140),
KEY order_msg_time(from_id,to_id,msg_time),
FOREIGN KEY (from_id) REFERENCES userlist(id),
FOREIGN KEY (to_id) REFERENCES userlist(id)
);



insert into userlist values("1",MD5('1'),'1970-1-1 00:00:00');
insert into settings values("1",0,0,0,0);
insert into userlist values("2",MD5('2'),'1970-1-1 00:00:00');
insert into settings values("2",0,0,0,0);
insert into userlist values("3",MD5('3'),'1970-1-1 00:00:00');
insert into settings values("3",0,0,0,0);
insert into userlist values("4",MD5('4'),'1970-1-1 00:00:00');
insert into settings values("4",0,0,0,0);
insert into userlist values("5",MD5('5'),'1970-1-1 00:00:00');
insert into settings values("5",0,0,0,0);


CREATE TABLE test(
id VARCHAR(32)
);
insert into test values("1");
insert into test values("2");
insert into test values("3");
insert into test values("4");
insert into test values("5");

