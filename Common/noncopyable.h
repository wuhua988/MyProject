#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

/*
 * �ѿ������캯���͸�ֵ�������Ϊ˽�У���Ϊ�˷�ֹ����ĸ���
 * �ѹ��캯��������������Ϊprotected���Ƿ�ֹ���ɱ���Ķ���
 */
class noncopyable {

protected:
	noncopyable() {

	}
	~noncopyable() {

	}

private:
	noncopyable(const noncopyable&);
	noncopyable &operator=(const noncopyable &);
};

#endif // __NONCOPYABLE_H__