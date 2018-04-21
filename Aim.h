#pragma once

#ifndef M_PI
#define M_PI          3.14159265358979323846
#endif
using namespace std;
Vec3 CalcAngleTest(Vec3 targ)
{
	Vec3 Angle = { 0.f, 0.f, 0.f };

	Camera* pCam = Camera::GetInstance();
	if (pCam)
	{
		//cout << pCam->CamPos.x << " " << pCam->CamPos.y << " " << pCam->CamPos.z << "\n";
		float hyp1 = distance3Df(pCam->CamPos, targ);
		Vec3 help = targ;
		help.y = pCam->CamPos.y;
		float katet1 = distance3Df(pCam->CamPos, help);

		Angle.y = acosf(katet1 / hyp1);
		if (pCam->CamPos.y > targ.y)
		{
			Angle.y = -Angle.y;
		}

		Vec3 help2 = help;
		help2.x = pCam->CamPos.x;
		help2.z = pCam->CamPos.z - katet1;
		float horda = distance3Df(help, help2);

		Angle.x = asinf(horda / (2.f * katet1)) * 2.f;

		if (pCam->CamPos.x < targ.x)
		{
			Angle.x = -Angle.x;
		}
	}

	return Angle;
}

void AngleNormalize(float* angles)
{
	if (angles[0] > 89.0f && angles[0] <= 180.0f)
		angles[0] = 89.0f;
	if (angles[0] > 180.f)
		angles[0] -= 360.f;
	if (angles[0] < -89.0f)
		angles[0] = -89.0f;
	if (angles[1] > 180.f)
		angles[1] -= 360.f;
	if (angles[1] < -180.f)
		angles[1] += 360.f;
	if (angles[2] != 0.0f)
		angles[2] = 0.0f;
}

void VectorSubtract(const Vec3& a, const Vec3& b, Vec3& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

void SmoothAimAngles(Vec3 MyViewAngles, Vec3 AimAngles, Vec3 &OutAngles, float Smoothing)
{
	VectorSubtract(AimAngles, MyViewAngles, OutAngles);

	AngleNormalize(OutAngles.GetBase());

	OutAngles.x = OutAngles.x / Smoothing + MyViewAngles.x;
	OutAngles.y = OutAngles.y / Smoothing + MyViewAngles.y;

	AngleNormalize(OutAngles.GetBase());
}

void XtoY(Vec3& vec)
{
	Vec3 tmp;
	tmp.x = vec.y;
	tmp.y = vec.x;
	vec = tmp;

}

void Aim(BaseEntity* mPlayer, Vec3 Pos)
{
	Vec3 Angle = CalcAngleTest(Pos);
   // std::cout << "angles " << Angle.x << " " << Angle.y << "\n";
	Angle.x *= 180.f / M_PI;
	Angle.y *= 180.f / M_PI;
	XtoY(Angle);

	Aimer* AnglesAim = Aimer::GetInstance();
	if (IsValidPtr(AnglesAim))
	{
		Vec3 mAngle(AnglesAim->Yaw, AnglesAim->Pitch, 0);
		float myyaw = mAngle.x;
		float mypitch = mAngle.y;

		while (mAngle.x > (M_PI * 2))
		{
			mAngle.x -= (M_PI * 2);
		}

		while (mAngle.x < (-(M_PI * 2)))
		{
			mAngle.x += (M_PI * 2);
		}





		mAngle.x *= 180.f / M_PI;
		mAngle.y *= 180.f / M_PI;
		XtoY(mAngle);

		Vec3 SetAngle;

		SmoothAimAngles(mAngle, Angle, SetAngle, Features::AimSpeed);//speed

		SetAngle.x /= 180.f / M_PI;
		SetAngle.y /= 180.f / M_PI;

		XtoY(SetAngle);



		if (myyaw > (M_PI))
		{
			while (abs(myyaw - SetAngle.x) > (M_PI / 2))
			{
				SetAngle.x += (M_PI);
			}
		}
		if (myyaw < (-M_PI))
		{
			while (abs(myyaw - SetAngle.x) > (M_PI / 2))
			{
				SetAngle.x -= (M_PI);
			}
			//myyaw +=(2* 3.141);
		}



		//std::cout << AnglesAim->Yaw << " " << AnglesAim->Pitch << "\n";
		AnglesAim->Yaw = SetAngle.x;
		AnglesAim->Pitch = SetAngle.y;
	}
	else
	{
		//fprintf(f, "not valid aimer\n");
	}
}




bool GetPredict(Vec3 Me1,Vec3 Targ1, Vector3 TargetSpeed,float bulletSpeed,Vec3 & out)
{

	Vector3 Me(Me1.x,Me1.y,Me1.z);
	Vector3 Target(Targ1.x, Targ1.y, Targ1.z);

	Me -= Target; // so the target is at 0|0|0

	float a = (TargetSpeed.x * TargetSpeed.x) + (TargetSpeed.y * TargetSpeed.y) + (TargetSpeed.z * TargetSpeed.z) - (bulletSpeed* bulletSpeed);
	float b = (-2 * Me.x * TargetSpeed.x) + (-2 * Me.y * TargetSpeed.y) + (-2 * Me.z * TargetSpeed.z);
	float c = (Me.x*  Me.x) + (Me.y*  Me.y) + (Me.z*  Me.z);

	Me += Target;  // get back the corret translation

	//Result r = MidnightSolver(a, b, c); // you dont have to create an extra function for this if you dont want
	//std::cout <<a <<" "<< c << "\n";
	double subsquare = b * b - 4 * a * c;
	float a2;
	float b2;
	if (subsquare < 0)
	{
		return 0;
	}
	else if (a != 0 && subsquare!=0)
	{
		a2 = (float)((-b + sqrtf(subsquare)) / (2 * a));
		b2 = (float)((-b - sqrtf(subsquare)) / (2 * a));
	}
	else
	{
		return false;
	}



										// now check if any solition is positive and which one is smaller
	if (a2 >= 0 && !(b2 >= 0 && b2 <a2))
	{
		// Aim at
		Vector3 aimPosition = Target + TargetSpeed * a2;
		out.x = aimPosition.x;
		out.y = aimPosition.y;
		out.z = aimPosition.z;
		return 1;
	}
	else if (b2 >= 0)
	{
		// Aim at
		Vector3 aimPosition = Target + TargetSpeed * b2;
		out.x = aimPosition.x;
		out.y = aimPosition.y;
		out.z = aimPosition.z;
		return 1;
	}
	return 0;

}



class AimbotPredictor
{
public:
	static Vec3 DoPrediction(const Vector3& ShootSpace, Vector3 AimPoint, const Vector3& MyVelocity, const Vector3& EnemyVelocity, const float& BulletSpeed, const float Gravity)
	{
		Vector3 RelativePos = (AimPoint - ShootSpace);
		Vector3 GravityVec = Vector3(0, fabs(Gravity), 0);
		//std::cout << "gabe\n";
		//std::cout << AimPoint.x << " " << AimPoint.y << " " << AimPoint.z << "\n";
		auto fApproxPos = [](Vector3& CurPos, const Vector3& Velocity, const Vector3& Accel, const float Time)->Vector3 {
			return CurPos + Velocity*Time + .5f*Accel*Time*Time;
		};

		//http://playtechs.blogspot.com/2007/04/aiming-at-moving-target.html
		double a = .25f * GravityVec.Dot(GravityVec);
		double b = EnemyVelocity.Dot(GravityVec);
		double c = RelativePos.Dot(GravityVec) + EnemyVelocity.Dot(EnemyVelocity) - (BulletSpeed * BulletSpeed);
		double d = 2.0f*(RelativePos.Dot(EnemyVelocity));
		double e = RelativePos.Dot(RelativePos);

		//Calculate time projectile is in air
		std::vector<double> Solutions;
		int NumSol = PolynomialSolver::SolveQuartic(a, b, c, d, e, Solutions);

		//find smallest non-negative real root
		float ShortestAirTime = 99999.0f;
		for (int i = 0; i < NumSol; i++)
		{
			float AirTime = Solutions[i];
			if (AirTime<0)
				continue;

			if (AirTime < ShortestAirTime)
				ShortestAirTime = AirTime;
		}
		//Extrapolate position on velocity, and account for bullet drop
		AimPoint = fApproxPos(AimPoint, EnemyVelocity, GravityVec, ShortestAirTime);
		//std::cout << AimPoint.x << " " << AimPoint.y << " " << AimPoint.z << "\n";
		
		return Vec3(AimPoint.x, AimPoint.y, AimPoint.z);
		//if (Zero.m_ZeroDistance == -1.0f)
		//	return 0.0f;

		//This is still an approximation, fix later
		//float ZeroAirTime = Zero.m_ZeroDistance / fabs(BulletSpeed);
		//float ZeroDrop = (.5f*fabs(Gravity)*ZeroAirTime*ZeroAirTime);
		//float Theta = atan2(ZeroDrop, Zero.m_ZeroDistance);
		//return Theta;
	}
};


