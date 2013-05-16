GIT INSTRUCTIONS

5 basic commands
- git status
		reports the status of your repository compared to the last time you pulled

- git pull
		pull down from the server any changes that have been checked in by others

- git add */specific files
		adds the specified files to staging for a check-in

- git commit -m "you message here"
		commits the files from the previous add to be checked in along with a message
		which contains a small notes of the changes that were made

- git push
		push your staged changes out to the server



Typical checkout
- git status
	to check if you need to commit anything before pulling

- git pull
	pull down changes



Typical check-in
- git status
	list all changes you have made

- git add myfile.c
	add myfile.c to be checked in

- git commit -m "implemented x in myfile.c"

- git push
	push changes out to server