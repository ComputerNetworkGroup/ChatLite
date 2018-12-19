//监听submit按钮
var change_pwd_btn = document.getElementById('change-pwd-btn');
change_pwd_btn.addEventListener('click', function () {
    var np1_value = document.getElementById('np1').value;
    var np2_value = document.getElementById('np2').value;
    console.log(np1_value);
    console.log(np2_value);
    document.getElementById('np1').value = '';
    document.getElementById('np2').value = '';

    //判断是否输入
    if (np1_value.length > 0 && np2_value.length > 0) {

        //判断前后两次密码是否一致
        if (np1_value != np2_value) {
            $('#error-msg').empty();
            $('#error-msg').append('<span style="color:red">Two Passwords are Inconsistent!</span>');
        }
        else {
            //如果新密码与原始密码一致
            if (true) {
                $('#error-msg').empty();
                $('#error-msg').append('<span style="color:red">New Password and Old Password are the Same!</span>');
            }
            else {

            }
        }
    }
    else {
        $('#error-msg').empty();
        $('#error-msg').append('<span style="color:red">Please Enter Your New Password and Repeat it!</span>');
    }
})