<?php
    $userId=$_POST['userId'];
    if($userId=="")
    {
        echo "empty msg";
    }else
    {
        $cmd="query_profile -c walotta_admin -u $userId";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'ticket.walotta.top',9988);
        socket_write($socket,strlen($cmd).$cmd);
        $res = socket_read($socket, 204800);
        socket_close($socket);
        if($res=="-1")echo "failed query";
        else
        {
            $returnArr=array();
            $msg=explode(" ",$res);
            $returnArr["userId"]=$msg[0];
            $returnArr["userName"]=$msg[1];
            $returnArr["mail"]=$msg[2];
            $returnArr["pri"]=$msg[3];

            $myJSON=json_encode($returnArr);
            echo $myJSON;
        }
    }
?>