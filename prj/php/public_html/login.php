<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
    <head>
        <title>
            登陆
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
                            登陆
                        </div>
                        <div class="list top">
                            <div class="name">
                                <span>
                                    登陆
                                </span>
                            </div>
                            <table width="100%" height="100%">
                                <tr>
                                    <td class="left">
                                        <div class="block" style="float:left;">
                                            <div class="head">
                                                主机控制面板
                                            </div>
                                            <table>
                                                <form enctype="application/x-www-form-urlencoded" action="http://cpanel.likebeta.tk/auth" method="post">
                                                    <tr>
                                                        <td class="left">
                                                            Email
                                                        </td>
                                                        <td class="color">
                                                            <input type="text" name="email" id="email" value="" placeholder="E-mail" />
                                                            </td>
                                                        </tr>
                                                        <tr>
                                                            <td class="left">
                                                                密码
                                                            </td>
                                                            <td class="color">
                                                                <input type="password" name="password" id="password" value="" placeholder="Password" />
                                                                </td>
                                                            </tr>
                                                            <tr>
                                                                <td class="left">
                                                                </td>
                                                                <td class="color">
                                                                    <input type="submit" name="submit" id="submit" value="登陆" />
                                                                    </td>
                                                                </tr>
                                                            </form>
                                                        </table>
                                                    </div>
                                                    <div class="block">
                                                        <div class="head">
                                                            Webmail
                                                        </div>
                                                        <table>
                                                            <form enctype="application/x-www-form-urlencoded" action="http://webmail.likebeta.tk" method="post">
                                                                <tr>
                                                                    <td class="left">
                                                                        Email
                                                                    </td>
                                                                    <td class="color">
                                                                        <input type="text" name="email" id="email" value="" placeholder="E-mail" />
                                                                        </td>
                                                                    </tr>
                                                                    <tr>
                                                                        <td class="left">
                                                                            密码
                                                                        </td>
                                                                        <td class="color">
                                                                            <input type="password" name="password" id="password" value="" placeholder="Password" />
                                                                            </td>
                                                                        </tr>
                                                                        <tr>
                                                                            <td class="left">
                                                                            </td>
                                                                            <td>
                                                                                <input type="submit" name="submit" id="submit" value="登陆" />
                                                                                </td>
                                                                            </tr>
                                                                        </form>
                                                                    </table>
                                                                </div>
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