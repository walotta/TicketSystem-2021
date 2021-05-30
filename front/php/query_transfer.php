<?php
    //echo "1";
    $is_time=$_POST["is_time"];
    $start_day=$_POST["start_day"];
    $start_month=$_POST["start_month"];
    $terminal_station=$_POST["terminal_station"];
    $start_station=$_POST["start_station"];
    //echo $is_time."\n".$start_day."\n".$start_month."\n".$terminal_station."\n".$start_station;
    if($is_time==""||$start_day==""||$start_month==""||$terminal_station==""||$start_station=="")
    {
        echo "empty msg";
    }else
    {
        if(strlen($start_day)==1)$start_day="0".$start_day;
        if(strlen($start_month)==1)$start_month="0".$start_month;
        $cmd="query_transfer -s $start_station -t $terminal_station -d $start_month-$start_day -p ";
        if($is_time=="true")$cmd.="time";
        else $cmd.="cost";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'ticket.walotta.top',9988);
        socket_write($socket,strlen($cmd).$cmd);
        $res = socket_read($socket, 204800);
        socket_close($socket);
        if($res=="-1")
        {
            echo "failed query";
        }else if($res=="0")
        {
            $transArr=array();
            $myJSON = json_encode($transArr);
            echo $myJSON;
        }else
        {
            //echo $res;
            $transArr=array();
            
            $returnArr=explode("\n",$res);
            for($i=0;$i<count($returnArr);$i++)
            {
                $allMsgArr=explode(" ",$returnArr[$i]);
                $temMsg["trainId"]=$allMsgArr[0];
                $temMsg["from"]=$allMsgArr[1];
                $temMsg["leaving_date"]=$allMsgArr[2];
                $temMsg["leaving_time"]=$allMsgArr[3];
                $temMsg["to"]=$allMsgArr[5];
                $temMsg["arriving_date"]=$allMsgArr[6];
                $temMsg["arriving_time"]=$allMsgArr[7];
                $temMsg["price"]=$allMsgArr[8];
                $temMsg["seat"]=$allMsgArr[9];
                array_push($transArr,$temMsg);
            }

            $myJSON = json_encode($transArr);
            echo $myJSON;
        }
    }
    
?>