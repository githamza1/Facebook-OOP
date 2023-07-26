#include <iostream>
#include <fstream>
using namespace std;

class helper
{
public:
	static int StringLenght(const char* str)
	{
		int i;
		for (i = 0; str[i] != '\0'; i++);
		return i;
	}
	static void StringCopy(char*& dest, const char*& src)
	{
		int size = StringLenght(src), i;
		for (i = 0; i < size; i++)
			dest[i] = src[i];
		dest[i] = '\0';
	}
	static char* GetStringFromBuffer(const char* str)
	{
		char* str1 = new char[StringLenght(str) + 1];
		StringCopy(str1, str);
		return str1;
	}
	static void printline()
	{
		for (int i = 0; i < 120; i++)
		{
			cout << "-";
		}
	}
};

template<class t>
class dealloc
{
public:
	static void dealloc1(t*& str)
	{
		if (str != NULL)
		{
			delete[] str;
			str = NULL;
		}
	}

	static void dealloc2(t**& str, int size) // for deletion of 2D array
	{
		int i = 0;
		while (i < size)
		{
			if (str[i] != NULL)
			{
				delete str[i];
			}
			i++;
		}
		delete[] str;
	}
};

class date //--------------------------------------------------------------------------------------------------->
{
private:
	int day = 0;
	int month = 0;
	int year = 0;
public:
	static date currentdate;
	void setcurrentdate(int d, int m, int y)  //static is setting fixed current date
	{
		currentdate.day = d;
		currentdate.month = m;
		currentdate.year = y;
	}
	date()
	{
		day = 0;
		month = 0;
		year = 0;
	}
	void readdatefromfile(ifstream& fin)
	{
		fin >> day;
		fin >> month;
		fin >> year;
	}
	void printdate()
	{
		cout << day << "/" << month << "/" << year << endl ;
	}
	bool isLatestPost()
	{
		if ((day == currentdate.day || day == currentdate.day - 1) && month == currentdate.month && year == currentdate.year)
		{
			return true;
		}
		else
			return false;
	}
	~date()
	{

	}
};
date date::currentdate;

class post;
class object //----------------------------------------------------------------------------------------->
{
public:
	virtual char* getID() = 0; // used in user and pages
	virtual char* getname() = 0;
	virtual void addpostintimeline(post* ptr){};
	virtual void printobj() {};
	~object()
	{
	}
};

class Comments //------------------------------------------------------------------------------------------->
{
private:
	char* cmntID;
	char* text;
	object* commenter;
	post* commentedon;
public:
	Comments()
	{
		cmntID = 0;
		text = 0;
		commenter = 0;
		commentedon = 0;
	}
	void readcommentID(ifstream& fin)
	{
		char temp[40];
		fin >> temp;
		cmntID = helper::GetStringFromBuffer(temp);
	}
	void readcommenttext(ifstream& fin)
	{
		char temp[100];
		fin.getline(temp, 100);
		text = helper::GetStringFromBuffer(temp);
	}
	void printcomment() 
	{
		cout << commenter->getID() << "  Commented : " << text << endl;
	}
	void setcommenter(object* ptr)
	{
		commenter = ptr;
	}
	void setcomment(object* objptr, const char* _text)
	{
		text = helper::GetStringFromBuffer(_text);
		commenter = objptr;
	}
	~Comments()
	{
		if (cmntID)
		{
			delete[] cmntID;
		}
		if (text)
		{
			delete text;
			text = 0;
		}
	}
};

class activity //------------------------------------------------------------------------------------------->
{
private:
	int activitytype;
	char* value;
public:
	activity()
	{
		activitytype = 0;
		value = NULL;
	}
	void loadactivityfromfile(ifstream& fin)
	{
		fin >> activitytype;
		char temp[40];
		fin.getline(temp, 40);
		value = helper::GetStringFromBuffer(temp);
	}
	void printactivity()
	{
		if (activitytype == 1)
		{
			cout << "Feeling ";
		}
		if (activitytype == 2)
		{
			cout << "Thinking about ";
		}
		if (activitytype == 3)
		{
			cout << "Making ";
		}
		if (activitytype == 4)
		{
			cout << "Celebrating ";
		}   // eg : celebrating a birthday
	}
	~activity()
	{
		if (value)
		{
			delete[] value;
		}
	}
};

class post //-------------------------------------------------------------------------------------------------->
{
private:
	char* postID;
	char* text;
	int posttype;
	int commentsonpost;
	int likesonpost;
	int postcount;
	int activitycount;
	object* sharedby;
	object** likedby;
	activity* _activity;
	date shareddate;
	Comments** comments;
public:
	post()
	{
		postID = NULL;
		text = NULL;
		posttype = 0;
		commentsonpost = 0;
		likesonpost = 0;
		postcount = 0;
		object* sharedby = NULL;
		object** likedby = NULL;
		activity* _activity = NULL;
		Comments** comments = NULL;
	}
	void readpostfromfile(ifstream& fin)
	{
		postcount++;
		fin >> posttype;
		char temp[100];
		fin >> temp;
		postID = helper::GetStringFromBuffer(temp);
		shareddate.readdatefromfile(fin);
		fin.ignore();
		fin.getline(temp, 100, '\n');
		text = helper::GetStringFromBuffer(temp);
		if (posttype == 2) // type 2 is the type with activity
		{
			activitycount++;
			_activity = new activity;
			_activity->loadactivityfromfile(fin);
		}
	}
	void printpost()
	{
		shareddate.printdate();
		cout << this->text << endl;
	}
	char* getpostID() // Simple getter for ID
	{
		return postID;
	}
	void setsharedby(object* sharedbyuser)
	{
		sharedby = sharedbyuser;
	}
	void setlikedby(object* ptr)
	{
		if (likesonpost == 0)
		{
			likedby = new object * [10];
		}
		if (likesonpost < 10)
		{
			likedby[likesonpost++] = ptr;
		}
		else
			cout << "No more likes can be added ! " << endl;
	}
	void addcommenttopost(Comments* cmntptr)
	{
		if (commentsonpost == 0)
		{
			comments = new Comments * [10];
		}
		if (commentsonpost < 10)
		{
			comments[commentsonpost++] = cmntptr;
		}
		else
			cout << "Unable to add anymore comments !" << endl;
	}
	void printcommentofpost() //printing comments on the post
	{
		for (int i = 0; i < commentsonpost; i++)
		{
			comments[i]->printcomment();
		}
	}
	void viewlikedlist()          //printing like list
	{
		for (int i = 0; i < likesonpost; i++)
		{
			likedby[i]->printobj();
			cout << endl;
		}
	}
	void printlastestpost()
	{
		if (shareddate.isLatestPost())
		{
			sharedby->printobj();
			if (posttype == 2)
			{
				_activity->printactivity();
			}
			else
				cout << "shared ";
			cout << "'" << text << "' .\n";
			for (int i = 0; i < commentsonpost; i++)
			{
				cout << "\t";
				comments[i]->printcomment();
				cout << "\n";
			}
			cout << endl;
		}
	}

	~post()
	{
		if (postID)
		{
			delete[] postID;
			postID = 0;
		}
		if (text)
		{
			delete[] text;
			text = 0;
		}
		dealloc<Comments>::dealloc2(comments, commentsonpost);
		//dealloc<activity>::dealloc1(_activity, activitycount);
	}
};

class Page : public object//-------------------------------------------------------------------------------->
{
private:
	char* ID;
	char* pgname;
	post** timeline;
	int pgposts;
public:
	Page()
	{
		ID = NULL;
		pgname = NULL;
		pgposts = 0;
	}

	char* getID()
	{
		return this->ID;
	}
	char* getpgID()
	{
		return ID;
	}
	void setpgID(char* c)
	{
		ID = c;
	}

	void readpgfromfile(ifstream& fin)
	{
		char temp[60];
		fin >> temp;
		ID = helper::GetStringFromBuffer(temp);
		fin.getline(temp, 60, '\n');  // using getline to read PG name of more than 2 words & \n move pointer to next line
		pgname = helper::GetStringFromBuffer(temp);
	}

	void outputPg()
	{
		cout << this->ID << " " << this->pgname << endl;
	}
	void printobj()
	{
		cout << ID << "->" << pgname << endl;
	}

	void printtimeline()
	{
		for (int i = 0; i < pgposts; i++)
		{
			timeline[i]->printlastestpost();
		}
	}

	void addpostintimeline(post* ptr)
	{
		if (pgposts == 0)
		{
			timeline = new post * [10];
		}
		if (pgposts < 10)
		{
			timeline[pgposts] = ptr;
			pgposts++;
		}
		else
			cout << "Unable to add post to page's timeline\n";
	}

	char* getname()
	{
		return this->pgname;
	}

	~Page()
	{
		if (ID)
		{
			delete[] ID;
		}
		if (pgname)
		{
			delete[] pgname;
		}
		dealloc<post>::dealloc2(timeline, pgposts);
	}
};

class user : public object //------------------------------------------------------------------------------------>
{
private:
	char* ID;
	char* firstname;
	char* lastname;
	int userFriends = 0;
	int userPages = 0;
	int userposts = 0;
	int usercount = 0;
	user** friendlist;
	Page** likedpages;
	post** timeline;
public:
	char** tempfriend;

	user()
	{
		ID = NULL;
		firstname = NULL;
		lastname = NULL;
		friendlist = NULL;
		timeline = NULL;
	}

	char* getID()
	{
		return this->ID;
	}

	char* getname()
	{
		return this->firstname;
	}

	void readuserfromfile(ifstream& fin)
	{
		usercount++;
		char temp[20];
		fin >> temp;
		ID = helper::GetStringFromBuffer(temp);
		fin >> temp;
		firstname = helper::GetStringFromBuffer(temp);
		fin >> temp;
		lastname = helper::GetStringFromBuffer(temp);
	}

	void printuser()
	{
		cout << this->ID << " " << this->firstname << " " << this->lastname << endl;
	}
	void printobj() // Child function
	{
		cout << ID << " " << firstname << " " << lastname;
	}
	void addfriend(user** ptr, int friendnum)
	{
		userFriends = friendnum;
		friendlist = new user * [userFriends];
		for (int i = 0; i < userFriends; i++)
		{
			friendlist[i] = ptr[i];
		}
	}
	void likepage(Page** ptr, int pagenum)
	{
		userPages = pagenum;
		likedpages = new Page * [pagenum];
		for (int i = 0; i < pagenum; i++)
		{
			likedpages[i] = ptr[i];
		}
	}

	void viewfriendlist()
	{
		for (int i = 0; i < userFriends; i++)
		{
			friendlist[i]->printuser();
		}
	}

	void viewlikedpages()
	{
		for (int j = 0; j < userPages; j++)
		{
			this->likedpages[j]->outputPg();
		}
	}

	void addpostintimeline(post* ptr)
	{
		if (userposts == 0)
			timeline = new post * [10];
		if (userposts < 10) {
			timeline[userposts] = ptr;
			userposts++;
		}
		else
			cout << "Unable to add post to users timeline\n";
	}
	void printtimeline()
	{
		for (int i = 0; i < userposts; i++)
		{
			timeline[i]->printpost();
		}
	}
	void homeview()
	{
		for (int i = 0; i < userFriends; i++)
		{
			friendlist[i]->printtimeline();
		}
		for (int i = 0; i < userPages; i++)
		{
			likedpages[i]->printtimeline();
		}
	}

	~user()
	{
		if (ID)
		{
			delete[] ID;
		}
		if (firstname)
		{
			delete[] firstname;
		}
		if (lastname)
		{
			delete[] lastname;
		}
		dealloc<post>::dealloc2(timeline, userposts);
	}
};

class memory :public post //----------------------------------------------------------------------->
{
private:
	post* originalpost;
	object* sharedby;
public:
	memory()
	{
		originalpost = 0;
		sharedby = 0;
	}
	void setmemory(post* ptr)
	{
		originalpost = ptr;
	}
	virtual void printmemory()
	{
		originalpost->printpost();
	}
	~memory()
	{
		if (originalpost)
		{
			delete[] originalpost;
		}
		if (sharedby)
		{
			delete[] sharedby;
		}
	}
};

class FB
{
private:
	int totalpages;
	int totalusers;
	int totalcomments;
	int totalposts;
	user** users;
	Page** pages;
	post** posts;
	Comments** comments;
	user* currentuser;
public:
	FB()
	{
		totalpages = 0;
		totalusers = 0;
		totalcomments = 0;
		totalposts = 0;
		pages = NULL;
		users = NULL;
		posts = NULL;
		comments = NULL;
	}
	user* searchuserbyID(const char* ptr)
	{
		int len = helper::StringLenght(ptr); // length of user ID for inner loop
		int x = 0;
		int i = 0;
		for (; i < totalusers; i++)
		{
			for (int j = 0; j < len; j++)
			{
				if (this->users[i]->getID()[j] == ptr[j])
				{
					x++;
				}
				else {
					x = 0;
				}
			}
			if (x == len)
			{
				break;
			}
		}
		return this->users[i];
	}

	Page* searchpagebyID(const char* pageptr)
	{
		for (int i = 0; i < totalpages; i++)
		{
			bool flag = strcmp(pageptr, pages[i]->getpgID());
			if (flag == 0)
			{
				return pages[i];
			}
		}
	}

	post* searchpostbyid(const char* idptrr)
	{
		int len = helper::StringLenght(idptrr); // length of post type for inner loop
		int x =0;
		for (int i = 0; i < totalposts; i++)
		{
			for (int j = 0; j < len; j++)
			{
				if (posts[i]->getpostID()[j] == idptrr[j])
				{
					x++;
				}
				else {
					x = 0;
				}
			}
			if (x==len)
			{
				return posts[i];
			}
		}
	}

	object* searchobj(const char* id)
	{
		object* ptr = 0;
		if (id[0] == 'u')
		{
			ptr = searchuserbyID(id);
		}
		if (id[0] == 'p')
		{
			ptr = searchpagebyID(id);
		}
		return ptr;
	}

	void setcurrentuser(const char* temp)
	{
		currentuser = searchuserbyID(temp);
	}

	void loaddata1(const char* file, const char* file2)
	{
		ifstream fin;
		ifstream fin2;
		fin.open(file);
		if (fin.is_open())
		{
			fin >> totalpages;
			pages = new Page * [totalpages];
			for (int i = 0; i < totalpages; i++)
			{
				pages[i] = new Page;
				pages[i]->readpgfromfile(fin);
			}
		}
		else cout << "Error in pages file opening \n";
		fin.close();

		fin2.open(file2);
		if (fin2.is_open())
		{
			fin2 >> totalusers;
			users = new user * [totalusers];
			char*** tempfriends;
			char*** templikedpg;
			char tempf[20][30];
			char tempg[20][30];
			tempfriends = new char** [totalusers];
			templikedpg = new char** [totalusers];
			for (int k = 0; k < totalusers; k++)
			{
				users[k] = new user;
				users[k]->readuserfromfile(fin2);
				/*users[k]->printuser();*/
				tempfriends[k] = new char* [10];
				int count = 0;
				for (int j = 0; tempf[k][0] != '-'; j++)
				{
					if (tempf[k] == "-1")
					{
						break;
					}
					else
					{
						tempfriends[k][j] = new char[10];
						fin2 >> tempf[k];
						tempfriends[k][j] = helper::GetStringFromBuffer(tempf[k]);
						count++;
					}
				}
				templikedpg[k] = new char* [10];
				for (int j = 0; tempg[k][0] != '-'; j++)
				{
					if (tempg[k] == "-1")
					{
						break;
					}
					else
					{
						templikedpg[k][j] = new char[10];
						fin2 >> tempg[k];
						templikedpg[k][j] = helper::GetStringFromBuffer(tempg[k]);
					}
				}
			}
			user* uptr[10];
			Page* pgptr[10];
			for (int i = 0; i < this->totalusers; i++) // using association
			{
				int j = 0;
				for (; tempfriends[i][j][0] == 'u'; j++)
				{
					uptr[j] = searchuserbyID(tempfriends[i][j]);
				}
				users[i]->addfriend(uptr, j);

				j = 0;
				for (; templikedpg[i][j][0] == 'p'; j++)
				{
					pgptr[j] = searchpagebyID(templikedpg[i][j]);
				}
				users[i]->likepage(pgptr, j);
			}
			// DEallocation of 3D users pointers
			//for (int i = 0; i < totalusers; i++)
			//{
			//	for (int j = 0; j < 10; j++)
			//	{
			//		delete[] tempfriends[i][j];

			//	}
			//	delete[] tempfriends[i];
			//}
			//	delete[] tempfriends;

			//	// deletion of 3D pages ptr
			//	// DEallocation of 3D users pointers
			//	for (int i = 0; i < totalpages; i++)
			//	{
			//		for (int j = 0; j < 10; j++)
			//		{
			//			delete[] templikedpg[i][j];

			//		}
			//		delete[] templikedpg[i];
			//	}
			//	delete[] templikedpg;

}
		else cout << "Error in users file opening \n";
		fin2.close();

		
	}
	void loaddata2(const char* file3, const char* file4)
	{
		ifstream fin3;
		ifstream fin4;
		fin4.open(file4);
		if (fin4.is_open())
		{

			fin4 >> totalposts;
			posts = new post * [totalposts];
			for (int i = 0; i < totalposts; i++)
			{
				posts[i] = new post;
				posts[i]->readpostfromfile(fin4);
				char userofpost[50];
				//--------------------formation of timeline------------------
				fin4 >> userofpost;
				object* sharedby = NULL;
				sharedby = searchobj(userofpost);
				posts[i]->setsharedby(sharedby);
				sharedby->addpostintimeline(posts[i]);
				object* likerofpost = NULL;
				while (userofpost[0] != '-')
				{
					fin4 >> userofpost;
					if (userofpost[0] != '-')
					{
						likerofpost = searchobj(userofpost);
						posts[i]->setlikedby(likerofpost);
					}
				}
			}
		}
		else cout << "Error in opening posts file \n";

		fin3.open(file3);
		if (fin3.is_open())
		{
			fin3 >> totalcomments;
			comments = new Comments * [totalcomments];
			int cmntcount = 0;
			for (int i = 0; i < totalcomments; i++)
			{
				cmntcount++;
				char tempcmnt[20];
				comments[i] = new Comments;
				comments[i]->readcommentID(fin3);

				fin3 >> tempcmnt;
				post* postptr = searchpostbyid(tempcmnt);
				postptr->addcommenttopost(comments[i]);

				fin3 >> tempcmnt;
				object* commenter = searchobj(tempcmnt);
				comments[i]->setcommenter(commenter);

				fin3 >> tempcmnt;
				comments[i]->readcommenttext(fin3);
			}
		}
		else cout << "Error posts comments opening \n";
	}

	void PostComment(const char* postID, const char* text)
	{
		Comments* newcomment = new Comments;
		newcomment->setcomment(currentuser, text);
		post* ptr = searchpostbyid(postID);
		ptr->addcommenttopost(newcomment);
	}

	void printpostcomment(const char* ptr)
	{
		post* tempptr;
		tempptr = searchpostbyid(ptr);
		tempptr->printcommentofpost();  //printing postcomment here calling in run
	}

	void viewpage(const char* page)
	{
		Page* pgptr;
		pgptr = searchpagebyID(page);
		pgptr->printtimeline();
	}

	void viewlikers(const char* ptrr)
	{
		post* currentpost;
		currentpost = searchpostbyid(ptrr);
		currentpost->viewlikedlist();
		cout << "Post Liked Successfully By Current User.\n";

	}

	void Run()
	{
		cout << "Current Date:\t";
		date::currentdate.setcurrentdate(15, 11, 2017);
		date::currentdate.printdate();

		setcurrentuser("u7");
		cout << "Current user is \n";
		currentuser->printuser();
		cout << "\nList of user friend :\n";
		currentuser->viewfriendlist();
		cout << "\nList of liked pages by user :\n";
		currentuser->viewlikedpages();

		cout << "\n------------------------View Home------------------------\n";
		cout << endl;
		currentuser->homeview();

		cout << "\n------------------------------Timeline-------------------------------\n\n";
		currentuser->printtimeline();

		cout << "\n------------------------ Liked post (hard code)------------------------\n\n";
		viewlikers("post5");

		cout << endl;
		cout << "\n----------------------------------Comments of post-----------------------------\n\n" ;
		printpostcomment("post5");

		cout << "\n\n------------------------Adding comment in post------------------------\n\n" ;
		PostComment("post8", "Thanks for wishes");
		printpostcomment("post8");

		cout << "\n\n------------------------Command : ViewPage------------------------\n\n" ;
		viewpage("p1");

		cout << "\n\n------------------------Command : Set current user u11------------------------\n\n" ;
		setcurrentuser("u11");
		currentuser->printuser();
		cout << " successfully set as Current User" << endl;

		cout << "\n----------------------------Command: ViewHome------------------------\n\n" ;
		currentuser->homeview();

		cout << "\n------------------------Command: ViewTimeline------------------------" << endl;
		currentuser->printtimeline();

	}

	~FB()
	{
		dealloc<user>::dealloc2(users, totalusers);
		dealloc<Page>::dealloc2(pages, totalpages);
		/*dealloc<post>::dealloc2(posts, totalposts);*/
		//dealloc<Comments>::dealloc2(comments, totalcomments);
	}
};

int main()
{
	FB obj;
	obj.loaddata1("pages.txt", "users.txt"); // phase 1 
	obj.loaddata2("comments.txt", "posts.txt"); // phase 2
	obj.Run();

	return 0;
}