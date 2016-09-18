这个ThinkPHP框架实现的think_message项目是fork from 2997215859@github.
<hr>

####为了较好理解该项目，通过阅读ThinkPHP_3.1.2官方手册，我知道了3.1及以前版本的框架大体结构，并且花了些时间学习ThinkPHP的关系模型。
####通过使用PHPStorm的正则搜索功能理出了需要的表的大致结构；
####后来得知该项目是赵正桐老师讲解的ThinkPHP_3.1系列教程李的课件代码，就重点听了<a href="http://www.tudou.com/programs/view/Tk3V2dYt16w/?spm=0.0.Tk3V2dYt16w.A.2J3UuQ">兄弟连新版ThinkPHP视频教程30.ThinkPHP 3.1.2######本节课知识点： 1.关联模型使用 2.分页的使用</a>,对该留言板项目有了更清晰的了解。
<hr>

##该项目需要的数据表的创建：
###数据库表前缀：tp_

###需要数据库1：message
####create table tp_user(
####id int(4) not null primary key auto_increment,
####username varchar(20) not null,
####password varchar(40) not null,
####sex varchar(4) not null default '1'
####);

###需要数据库2：message
####create table tp_message(
####id int(10) not null primary key auto_increment,
####title varchar(40) not null,
####content varchar(60) not null,
####time int(20) not null,
####uid int(4) not null
####);

###显然数据库的数据字节数我设置的不好，因为我目前不算熟悉，设置得大点就大点吧，之后再优化。我还需要多看看其他项目，时不我待。

#为方便测试其他项目，我已把本地ceontos上的mysql用户root密码改我最中意的密码。如果要再测试该项目，需要适配这个问题。
