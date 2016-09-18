<?php
// 本类由系统自动生成，仅供测试用途
class RegisterAction extends Action {
    public function index(){
    	$m = M('User');
    	var_dump($m->select());
    }
}