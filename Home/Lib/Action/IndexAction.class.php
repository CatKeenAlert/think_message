<?php
// 本类由系统自动生成，仅供测试用途
class IndexAction extends CommonAction {
    public function index(){
    	$this->display('Index:index');
    }
    public function top()
    {
    	$this->display();
    }
    public function left()
    {
        import('ORG.Util.Page');// 导入分页类
        $m = D('Message');

        $count = $m->count();// 查询满足要求的总记录数
        $Page = new Page($count,2);// 实例化分页类传入总记录数和每页显示的记录数
        $Page->setConfig('header','条记录');//必须要在show之前生成
        $show = $Page->show();// 分页显示输出
        $arr = $m->relation(true)->limit($Page->firstRow.','.$Page->listRows)->select();
        //dump($arr);
        //exit();
        $this->assign('list',$arr);

        $this->assign('page',$show);// 赋值分页输出
    	$this->display();
    }
    public function right()
    {
    	$this->display();
    }
}