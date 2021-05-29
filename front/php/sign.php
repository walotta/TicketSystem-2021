<?php
    //echo "1";
    $id=$_POST["userId"];
    $psw=$_POST["psw"];
    $name=$_POST["name"];
    $mail=$_POST["mail"];
    if($id==""||$psw==""||$name=="")
    {
        echo "-1";
    }else
    {
        $cmd="add_user -c walotta_admin -u $id -p $psw -n $name -m $mail -g 1";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'ticket.walotta.top',9988);
        socket_write($socket,strlen($cmd).$cmd);
        $res = socket_read($socket, 204800);
        socket_close($socket);
        if($res=="0")
        {
            //set cookie
            $privilege=1;
            ini_set('session.gc_maxlifetime', 3600);
            $expire = ini_get('session.gc_maxlifetime');
            session_start();
            setcookie('PHPSESSID', session_id(), time() + $expire);
            setcookie('username', $id, time() + $expire);
            setcookie('privilege', $privilege, time() + $expire);
            if(isset($_SESSION['username']) && ($_SESSION['username'] == $username))
            {
 			 	exit("您已经登入了，请不要重新登入！用户名：".$_SESSION['username']."{$expire}");
 			}
            else
            {
 				$_SESSION['username'] = $username_cookie;
                $_SESSION['privilege'] = $privilege;
 			}
            //login
            $cmd="login -u $id -p $psw";
            $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
            socket_connect($socket,'ticket.walotta.top',9988);
            socket_write($socket,strlen($cmd).$cmd);
            $res = socket_read($socket, 204800);
            socket_close($socket);
            echo "1";
        }else
        {
            echo "0";
        }

        //echo $cmd;
    }
    
?>