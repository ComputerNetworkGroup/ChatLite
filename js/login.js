//监听login按钮，显示login card
document.getElementById('login').addEventListener('click', function () {
    document.getElementById('login-card').classList.add('is-active');
})

//监听submit按钮
document.getElementById('submit-btn').addEventListener('click', function () {
    var account_value = document.getElementById('account').value;
    var password_value = document.getElementById('password').value;
    document.getElementById('account').value = '';
    document.getElementById('password').value = '';

    //判断是否输入account和password
    if (account_value.length > 0 && password_value.length > 0) {
        //如果accont和password都已输入
        //判断该账户是否存在

        var 
        
        if (false) {
            //如果存在

        }
        else {
            //如果不存在
            $('#error-msg').empty();
            $('#error-msg').append('<span style="color:red">Account or Password Error!</span>')
        }
    }
    else {
        //如果account和password未输入
        $('#error-msg').empty();
        $('#error-msg').append('<span style="color:red">Please Enter Your Account or Password!</span>')
    }
})

document.getElementById('close-btn').addEventListener('click', function() {
    document.getElementById('login-card').classList.remove('is-active');
})