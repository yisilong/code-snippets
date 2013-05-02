<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
    <head>
        <title>
            联系我们
        </title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
        <meta http-equiv="content-script-type" content="text/javascript" />
        <meta http-equiv="content-style-type" content="text/css" />
        <link href="style.css" media="screen" rel="stylesheet" type="text/css" />
    </head>
    <body>
        <div id="bef-main">
            <div id="main">
                <?php include("include_header.php"); ?>
                <div id="mid">
                    <div id="side-1">
                        <div class="main-menu">
                            联系我们
                        </div>
                        <div class="list top">
                            <div class="name">
                                <span>
                                    联系我们
                                </span>
                            </div>
                            <table width="100%" height="100%">
                                <tr>
                                    <td class="left">
                                        <form action="mailto:huangdi915103@gmail.com" method="post">
                                            <table>
                                                <tr>
                                                    <td>
                                                        <font face="arial" size="2">
                                                            姓名：
                                                        </font>
                                                    </td>
                                                    <td>
                                                        <input name="us_name" size="20">
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td>
                                                            <font face="arial" size="2">
                                                                Email:
                                                            </font>
                                                        </td>
                                                        <td>
                                                            <input name="email_id" size="20" value=>
                                                            </td>
                                                        </tr>
                                                        <tr>
                                                            <td>
                                                                <font face="arial" size="2">
                                                                    你的问题简短描述：
                                                                </font>
                                                            </td>
                                                            <td>
                                                                <input name="bquestion" size="20">
                                                                </td>
                                                            </tr>
                                                            <tr>
                                                                <td>
                                                                    <font face="arial" size="2">
                                                                        详实描述你的问题：
                                                                    </font>
                                                                </td>
                                                                <td>
                                                                    <textarea name="question" rows="3" wrap="virtual" cols="30">
                                                                    </textarea>
                                                                </td>
                                                            </tr>
                                                        </table>
                                                        <br>
                                                        <input type="submit" value="联系我们" name="submit">
                                                            <input type="reset" value="重写" name="reset">
                                                            </form>
                                                        </td>
                                                    </tr>
                                                </table>
                                            </div>
                                        </div>
                                        <?php include("include_menu.php"); ?>
                                    </div>
                                </div>
                                <?php include("include_footer.php"); ?>
                            </div>
                        </html>