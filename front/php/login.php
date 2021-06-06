<?php
    //echo "1";
    include('function_pool.php');
    $id=$_POST["userId"];
    $psw=$_POST["psw"];
    if($id==""||$psw=="")
    {
        echo "-1";
    }else
    {
        $cmd="login -u $id -p $psw";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'ticket.walotta.top',9988);
        socket_write($socket,strlen($cmd).$cmd);
        $res = socket_read($socket, 204800);
        socket_close($socket);
        if($res=="0")
        {
            //set cookie
            $privilege=query_privilege($id);
            //ini_set('session.gc_maxlifetime', 3600);
            //$expire = 3600;//ini_get('session.gc_maxlifetime');
            //setcookie('username', $id, time() + $expire);
            //setcookie('PHPSESSID', session_id(), time() + $expire);
            //setcookie('privilege', $privilege, time() + $expire);
            /*session_start();
            if(isset($_SESSION['username']) && ($_SESSION['username'] == $username))
            {
 			 	exit("您已经登入了，请不要重新登入！用户名：".$_SESSION['username']."{$expire}");
 			}
            else
            {
 				$_SESSION['username'] = $username_cookie;
                $_SESSION['privilege'] = $privilege;
 			}*/
            echo "$privilege";
        }else
        {
            echo "0";
        }

        //echo $cmd;
    }
    
?>