<?php
    $userId=$_POST['userId'];
    $trainId=$_POST['trainId'];
    $date=$_POST['date'];
    $from=$_POST['from'];
    $to=$_POST['to'];
    $wait=$_POST['wait'];
    $number=$_POST['number'];

    if($number=="")
    {
        echo "-1";
    }else
    {
        $cmd="buy_ticket -u $userId -i $trainId -d $date -n $number -f $from -t $to";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'ticket.walotta.top',9988);
        socket_write($socket,strlen($cmd).$cmd);
        $res = socket_read($socket, 204800);
        socket_close($socket);
        echo $res;
    }
?>