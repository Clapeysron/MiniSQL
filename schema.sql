drop table prereq;
drop table time_slot;
drop table advisor;
drop table takes;
drop table student;
drop table teaches;
drop table section;
drop table instructor;
drop table course;
drop table department;
drop table classroom;

create table classroom
	(building		varchar(15),
	 room_number		varchar(7),
	 capacity		int
	);

create table department
	(dept_name		varchar(20), 
	 building		varchar(15), 
	 budget		    int
	);

create table course
	(course_id		varchar(8), 
	 title			varchar(50), 
	 dept_name		varchar(20),
	 credits		int
	);

create table instructor
	(ID			varchar(5), 
	 name			varchar(20), 
	 dept_name		varchar(20), 
	 salary			varchar(5)
	);

create table section
	(course_id		varchar(8), 
         sec_id			varchar(8),
	 semester		varchar(6), 
	 year			varchar(5),
	 building		varchar(15),
	 room_number		varchar(7),
	 time_slot_id		varchar(4)
	);

create table teaches
	(ID			varchar(5), 
	 course_id		varchar(8),
	 sec_id			varchar(8), 
	 semester		varchar(6),
	 year			varchar(5)
	);

create table student
	(ID			varchar(5), 
	 name			varchar(20), 
	 dept_name		varchar(20), 
	 tot_cred		varchar(5)
	);

create table takes
	(ID			varchar(5), 
	 course_id		varchar(8),
	 sec_id			varchar(8), 
	 semester		varchar(6),
	 year			varchar(5),
	 grade		        varchar(2)
	);

create table advisor
	(s_ID			varchar(5),
	 i_ID			varchar(5)
	);

create table time_slot
	(time_slot_id		varchar(4),
	 day			varchar(1),
	 start_hr		varchar(1),
	 start_min		varchar(1),
	 end_hr			varchar(1),
	 end_min		varchar(1)
	);

create table prereq
	(course_id		varchar(8), 
	 prereq_id		varchar(8)
	);