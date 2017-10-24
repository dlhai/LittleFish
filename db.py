from flask_sqlalchemy import SQLAlchemy
from flask import Flask
app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = "sqlite:///./chx.db"
db = SQLAlchemy(app)

class cLass(db.Model):
    __tablename__ = 'class'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(10))

class course(db.Model):
    __tablename__ = 'course'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(10))

class Student(db.Model):
    __tablename__ = 'student'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(10))
    cLass = db.Column(db.Integer, db.ForeignKey('class.id'))
    sage = db.Column(db.Integer)

class enroll(db.Model):
    __tablename__ = 'enroll'
    id = db.Column(db.Integer, primary_key=True)
    course = db.Column(db.Integer, db.ForeignKey('course.id'))
    student = db.Column(db.Integer, db.ForeignKey('student.id'))
    score = db.Column(db.String(10))
