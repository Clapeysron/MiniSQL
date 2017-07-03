drop table student;
create table student 
(
    id int,
    name varchar(32),
    score float
);
insert into student values (1, "Wei Jiarong", 59.9);
select * from student;
insert into student values (2, "John", 95.0);
insert into student values (3, "Lucy", 45.3);
insert into student values (4, "Jack", 80.0);
insert into student values (5, "Tom", 59.9);
select * from student;
select * from student where id=1;
select * from student where name="John";
select * from student where score=59.9;
select * from student where id!=1;
select * from student where name>="Lucy";
select * from student where score<90.0;
select * from student where score<90;
select * from student where id!=1 and score<90.0;
select * from student where score between 70.0 and 90.0;
select id,name from student where score between 70.0 and 90.0;
delete from student;
drop table student;
select * from student;
show tables;
