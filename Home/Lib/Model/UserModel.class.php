<?php 
class UserModel extends Model{
	protected $_validate = array(
		array('username','require','用户名必须！'), //默认情况下用正则进行验证
		array('username','/^\w{6,}$/','用户名必须六个字以上！',1,'regex',0), //默认情况下用正则进行验证
		array('password','require','密码必须！'), //默认情况下用正则进行验证
	   	array('code','require','验证码必须！'), //默认情况下用正则进行验证
	    array('username','','帐号名称已经存在！',0,'unique',1), // 在新增的时候验证name字段是否唯一
	    array('repassword','password','确认密码不正确',1,'confirm'), // 验证确认密码是否和密码一致
	    array('code','checkCode','验证码不一致',0,'callback',1),
	    //验证验证码
	);
	protected function checkCode($code)
	{
		if(md5($code)!=$_SESSION['verify']){
			return false;
		}else{
			return true;
		}
	}
}
?>