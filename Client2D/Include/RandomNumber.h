#pragma once

// ������ ������ �ϰ� ������ SeedRandomUInt()�� ���� ����
// �׷��� ������ ���� �õ�
// �õ带 ���� �Է��ϰ� ������ SetSeed
class RandomNumber
{
public:
	static void Init();
	static unsigned int SeedRandomUInt();
	static unsigned int TimeRandomUInt();
	static void SetSeed(unsigned int seed);
	static int GetRandomNumber(int min, int max);

private:
	RandomNumber() {}
	~RandomNumber() {}

private:
	static int mSeed;
	static int mTimeSeed;

};