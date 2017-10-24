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
    cLass = db.Column(db.Integer, db.ForeignKey('cLass.id'))
    sage = db.Column(db.Integer)

class enroll(db.Model):
    __tablename__ = 'enroll'
    course = db.Column(db.Integer, db.ForeignKey('course.id'))
    student = db.Column(db.Integer, db.ForeignKey('student.id'))
    score = db.Column(db.String(10))
