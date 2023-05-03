这里是一些关于git的命令

1. 连接到github
ssh -T git@github.com       
2. 新建一个分支
git branch <name>
3. 切换到某一分支
git checkout <name>
4. 查看用户名/邮箱
git config --global user.name/user.email
5. 强制推送同步
git push -f origin master
6. 强制回到某个提交
git reset --hard <sha>   
7. 添加远程连接
git remote add origin git@121.40.169.197:/home/git/gouwu/gouwu.git  
8. 设置用户名/邮箱
 git config user.email frappuccino2319@qq.com  
