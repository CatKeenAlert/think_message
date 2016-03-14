<?php 
class RegisterAction extends Action{
	public function index()
	{
		$this->display();
	}
	public function checkname()
	{
		$username= $_GET['username'];
		$m = M('User');
		$where['username'] = $username;
		$count = $m->where($where)->count();
		if($count){
			echo "该用户已经存在";
		}
	}
	public function doRegister($value='')
	{
		# code...
		// $username = $_POST['username'];
		// $password = $_POST['password'];
		// $repassword = $_POST['repassword'];
		// $sex = $_POST['sex'];
		// $data['username'] = $username;
		// $data['password'] = $password;
		// $data['sex'] = $sex;
		$m = D('User');
		if(!$m->create()){
			$this->error($m->getError());
		}
		//$lastid = $m->add($data);
		$lastid = $m->add();
		if($lastid){
			$this->redirect("Index/index");
		}else{
			$this->error("用户注册失败");
		}
	}
}

 ?>