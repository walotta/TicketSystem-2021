<?php
    $userId=$_POST['userId'];
    if($userId=="")
    {
        echo "empty msg";
    }else
    {
        $cmd="query_order -u $userId";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'ticket.walotta.top',9988);
        socket_write($socket,strlen($cmd).$cmd);
        $res = socket_read($socket, 204800);
        socket_close($socket);
        if($res=="-1")echo "failed query";
        else
        {
            $ansArr=array();
            $returnAll=explode("\n",$res);
            
            for($i=1;$i<count($returnAll);$i++)
            {
                $msg=explode(" ",$returnAll[$i]);
                $sonNode['status']=substr($msg[0],1,strlen($msg[0])-2);
                $sonNode['trainId']=$msg[1];
                $sonNode['from']=$msg[2];
                $sonNode['leaving_date']=$msg[3];
                $sonNode['leaving_time']=$msg[4];
                $sonNode['to']=$msg[6];
                $sonNode['arriving_date']=$msg[7];
                $sonNode['arriving_time']=$msg[8];
                $sonNode['price']=$msg[9];
                $sonNode['seat']=$msg[10];
                array_push($ansArr,$sonNode);
            }

            $myJSON=json_encode($ansArr);
            echo $myJSON;
        }
    }
?>