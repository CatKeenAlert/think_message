<?php
// 本类由系统自动生成，仅供测试用途
class LoginAction extends Action {
    public function index(){
    	$m = M('User');
    	var_dump($m->select());
    }
    public function login()
    {
    	$this->display();
    }
    public function do_login()
    {
    	$username = $_POST['username'];
    	$password = $_POST['password'];
    	$code = $_POST['code'];
    	if($_SESSION['verify'] != md5($code)){
    		$this->error("验证码错误");
    	}
    	$m = M('User');
    	$data['username'] = $username;
    	$data['password'] = $password;
    	$arr = $m->field('id')->where($data)->count();
        $arr2 = $m->field('id')->where($data)->find();
    	if($arr){
    		//有记录，则跳转
    		$_SESSION['username'] = $username;
    		$_SESSION['id'] = $arr2['id'];
    		$this->success('登陆成功',U('Index/index'));
    	}else{
    		$this->error('该用户不存在');
    	}
    }
    public function login_off()
    {
		$_SESSION = array();
		if(isset($_COOKIE[session_name()])){
			setcookie(session_name(),'',time()-1,'/');
		}
		session_destroy();
		$this->redirect("Index/index");
    }
}