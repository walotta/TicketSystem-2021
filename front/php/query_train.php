<?php
    //echo "1";
    $trainId=$_POST["trainId"];
    $start_day=$_POST["start_day"];
    $start_month=$_POST["start_month"];
    if($trainId==""||$start_day==""||$start_month=="")
    {
        echo "empty msg";
    }else
    {
        if(strlen($start_day)==1)$start_day="0".$start_day;
        if(strlen($start_month)==1)$start_month="0".$start_month;
        $cmd="query_train -i $trainId -d $start_month-$start_day";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'ticket.walotta.top',9988);
        socket_write($socket,strlen($cmd).$cmd);
        $res = socket_read($socket, 204800);
        socket_close($socket);
        if($res=="-1")
        {
            echo "failed query";
        }else
        {
            //echo $res;
            $transArr=array();
            $returnArr=explode("\n",$res);
            $baseMagArr=explode(" ",$returnArr[0]);
            $transArr["trainId"]=$baseMagArr[0];
            $transArr["type"]=$baseMagArr[1];
            $stationArr=array();
            for($i=1;$i<count($returnArr);$i++)
            {
                $allMsgArr=explode(" ",$returnArr[$i]);
                $temMsg["station"]=$allMsgArr[0];
                $temMsg["arrive_date"]=$allMsgArr[1];
                $temMsg["arrive_time"]=$allMsgArr[2];
                $temMsg["leaving_date"]=$allMsgArr[4];
                $temMsg["leaving_time"]=$allMsgArr[5];
                $temMsg["price"]=$allMsgArr[6];
                $temMsg["seat"]=$allMsgArr[7];
                array_push($stationArr,$temMsg);
            }
            $transArr["station"]=$stationArr;

            $myJSON = json_encode($transArr);
            echo $myJSON;
        }
    }
    
?>