$(function () {
    $('.login-form-input').blur(function () {
        if ($(this).val().length > 0) {
            $(this).addClass('active');
        } else {
            $(this).removeClass('active');
        }
        // 判断是否 account 和 password 内容
        if ($('#account').val().length > 0 && $('#password').val().length > 0) {
            $('.login-form-wrapper-submit').addClass('animated');
        } else {
            $('.login-form-wrapper-submit').removeClass('animated');
        }
    });
});

//显示登录卡片
function show_login_card() {
    $('.login-card').show();
    $('#cover').css('display','block');
    $('#cover').css('height',document.body.clientHeight+'px');
}

//监听login按钮
var login_btn = document.getElementById('login');
login_btn.addEventListener('click', function() {
    show_login_card();
})

//监听submit按钮
var submit_btn = document.getElementById('submit-btn');
submit_btn.addEventListener('click', function() {
    var account_value = document.getElementById('account').value;   //获取用户名
    var password_value = document.getElementById('password').value; //获取密码
    document.getElementById('account').value = '';                  //将用户名input置空
    document.getElementById('password').value = '';                 //将密码input置空

    if (account_value.length > 0 && password_value.length > 0) {
        //查询该账户是否存在
        if (false) {

        }
        else {
            $('#login-error').empty();
            $('#login-error').append('<span style="color:red">Account or Password Error!</span>');
        }
    }
    else {
        $('#login-error').empty();
        $('#login-error').append('<span style="color:red">Please Enter Your Account or Password!</span>');
    }

})