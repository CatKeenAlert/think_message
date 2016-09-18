<?php
class HelloAction extends Action{
    public function index(){

        echo 'hello, world!';
        echo '<br>';

        echo '如果能看到该行文字，则说明该方法调用的是：'
            .__ROOT__.'下的'
            .__APP__.'应用下的'
            .__MODULE__.'模块下的'
            .__CONTROLLER__.'控制器下的'
            .__ACTION__.'操作）方法。'.'以上所显示节点都引用自系统变量(魔术变量?)动态生成。';

        echo '<br><br><hr><br><br>';
        $dirs = array(
            '__ROOT__' => __ROOT__,  // 当前网站地址
            '__APP__'  => __APP__,   // 当前应用地址
            '__MODULE__' => __MODULE__,
            '__ACTION__' => __ACTION__, // 当前操作地址
            '__SELF__' => __SELF__,  // 当前页面地址
            '__CONTROLLER__'=> __CONTROLLER__,
            '__URL__' => __CONTROLLER__,
            '__PUBLIC__' => __ROOT__.'/Public',// 站点公共目录
        );
        echo '这是THINKPHP提供的部分默认变量(魔术变量?)，现在通过dump()函数以数组方式呈现：';
        dump($dirs);
    }
}
?>