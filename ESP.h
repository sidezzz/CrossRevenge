#pragma once

Vec3 Predicted[100];
Vec3 Predicted2D[100];
int PredictCount=0;
void ESP()
{
	//std::cout << "in\n";
	//fprintf(f, "%d %d\n", PredictCount, weaponsCount); fflush(f);
	CGameLogic* pGameLogic = CGameLogic::GetInstance();//(CGameLogic*)(GetModuleHandle(0)+0x1856B40);
	PredictCount = 0;
	weaponsCount = 0;
	if (IsValidPtr(pGameLogic))
	{
		BaseEntity* pLocalPlayer = pGameLogic->GetLocalPlayer();
		if (IsValidPtr(pLocalPlayer))
		{
			//DrawDebugLine(Vec3(0, 0, 0), Vec3(10, 10, 10), 0xff0000);
			EntityContainer* pEntityCont = EntityContainer::GetInstance();
			if (IsValidPtr(pEntityCont))
			{
				if (IsValidPtr(pEntityCont->BaseEntitysArray))
				{
					/*Vec3 del;
					del.y = 500;
					for (int a = 0; a < 400; a++)
					{
						Weapons[a] = del;
						Weapons3D[a].coord = del;
						Weapons3D[a].Pid = 0;
					}*/
					Dumped(MyWeap, pLocalPlayer->GetBoneByID(0)->matrix, pLocalPlayer,pLocalPlayer);
					//fprintf(f, "my weap- %f %f %f\n", MyWeapon.x, MyWeapon.y, MyWeapon.z); fflush(f);
					for (int Counter = 0; Counter < pEntityCont->EntityCount; Counter++)
					{
						BaseEntity* pEntity = pEntityCont->BaseEntitysArray[Counter];
						if (IsValidPtr(pEntity) && (pEntity != pLocalPlayer || Features::SelfRender))
						{
							if (Features::Predict && pEntity->TeamNum != pLocalPlayer->TeamNum)
							{
								
								if (IsValidPtr(pEntity->pBone->pSpeed))
								{
									//fprintf(f, "%s %d\n", pEntity->Name, pEntity->PartsCount); fflush(f);
									Vec3 help = pEntity->GetBoneByID(0)->Coord;
									help.y += Features::Yoffset;
									//std::cout << pEntity->pBone->pSpeed->Velocity.x << " " << pEntity->pBone->pSpeed->Velocity.y << " " << pEntity->pBone->pSpeed->Velocity.z << "\n";
									//if (GetPredict(pLocalPlayer->GetBoneByID(0)->Coord, pEntity->GetBoneByID(0)->Coord, pEntity->pBone->pSpeed->Velocity, (float)Features::PredictSpeed, Predicted[PredictCount]))
									if (Features::Gravity)
									{
										
										Predicted[PredictCount] = AimbotPredictor::DoPrediction(MyWeapon, help,
											pLocalPlayer->pBone->pSpeed->Velocity, pEntity->pBone->pSpeed->Velocity, Features::PredictSpeed, Features::GravityLevel);
										//fprintf(f, "Predicted- %f %f %f\n\n", Predicted[PredictCount].x, Predicted[PredictCount].y, Predicted[PredictCount].z); fflush(f);
									}
									else
									{
										GetPredict(pLocalPlayer->GetBoneByID(0)->Coord, pEntity->GetBoneByID(0)->Coord, pEntity->pBone->pSpeed->Velocity, (float)Features::PredictSpeed, Predicted[PredictCount]);
									}
									if (WorldToScreen(Predicted[PredictCount], Predicted2D[PredictCount]))
									{
										DrawDebugLine(pEntity->GetBoneByID(0)->Coord, Predicted[PredictCount], Features::OtherColor);
										PredictCount++;

									}
								}
								else
								{
									//fprintf(f, "not valid speed\n"); fflush(f);
								}

							}
							if (pEntity->Id == TargetLockValue)
							{
								Box2D(pEntity, Features::LockColor);
							}
							if (Features::Boxes)
							{
								Box22(pEntity->GetBoneByID(0)->matrix, pEntity->GetBoneByID(0)->Coord, 1.5, 0.8,Features::BoxColor);
							}
							if (pEntity->TeamNum != pLocalPlayer->TeamNum || Features::SelfRender)
							{
								Dumped(All, pEntity->GetBoneByID(0)->matrix, pEntity, pLocalPlayer);
							}
							if (Features::DrawStrings)
							{
								if (Features::DrawStringsForAlly || ((pEntity->TeamNum != pLocalPlayer->TeamNum) && !Features::DrawStringsForAlly))
								{
									Vec3 ent;
									if (WorldToScreen(pEntity->GetBoneByID(0)->Coord, ent))
									{
										if (Features::RangeToTarget)
										{
											DrawTextIm(ent.x, ent.y, Features::TextColor, "%d [%dm]", (int)pEntity->CurrentHealth, (int)distance3Df(pEntity->GetBoneByID(0)->Coord, pLocalPlayer->GetBoneByID(0)->Coord));
										}
										else
										{
											DrawTextIm(ent.x, ent.y, Features::TextColor, "%d", (int)pEntity->CurrentHealth);
										}
									}
								}
							}
						}
					}
				}
				else
				{
					//fprintf(f, "not valid baseentityarray\n"); fflush(f);
				}
			
				
				if (GetAsyncKeyState(0x52) != 0 && Features::LockMode)
				{
					Vec3 min;
					LockClass min2;
				    min2.coord.y = 500;
					Vec3 screen;
					screen.x = (Features::ScreenX / 2);
					screen.y = Features::ScreenY/2.42;
					int dist = 2000;
					if (weaponsCount != 0)
					{
						min = Weapons[0];

						for (int f = 0; f < weaponsCount; f++)
						{
							//std::cout << Weapons3D[f].coord.x << " " << Weapons3D[f].coord.y << " " << Weapons3D[f].coord.z << "\n";
							if (dist > distance2D(Weapons[f], screen))
							{
								min = Weapons[f];
								dist = distance2D(min, screen);
								min2 = Weapons3D[f];

							}
						}
					}
					if (min2.coord.y != 500)
					{

						if (Features::SightDistance > distance2D(min, screen))
						{
							TargetLockValue = min2.Pid;
						}
					}
					//TargetLockValue=pE
				}
				else if (GetAsyncKeyState(0x10) != 0)
				{
					Vec3 minV(0, 500, 0);
					Vec3 min2(0, 500, 0);
					Vec3 screen;
					screen.x = (Features::ScreenX / 2);
					screen.y = (Features::ScreenY / 2.42);
					int dist = 2000;
					if (weaponsCount != 0 &&!Features::Predict)
					{
						minV = Weapons[0];

						for (int f = 0; f < weaponsCount; f++)
						{
							//std::cout << Weapons[f].x << " " << Weapons[f].y << " " << Weapons[f].z << "\n";
							if (!Features::LockMode)
							{
								if (dist > distance2D(Weapons[f], screen))
								{
									minV = Weapons[f];
									dist = distance2D(minV, screen);
									min2 = Weapons3D[f].coord;

								}
							}
							else
							{
								if (dist > distance2D(Weapons[f], screen) && Weapons3D[f].Pid == TargetLockValue)
								{
									minV = Weapons[f];
									dist = distance2D(minV, screen);
									min2 = Weapons3D[f].coord;
								}
							}
						}
					}
					else if (Features::Predict)
					{
						for (int f = 0; f < PredictCount; f++)
						{
							if (dist > distance2D(Predicted2D[f], screen))
							{
								minV = Predicted2D[f];
								dist = distance2D(minV, screen);
								min2 = Predicted[f];

							}
						}
					}
					if (min2.y != 500)
					{

							if (Features::SightDistance> distance2D(minV, screen))
							{
								//fprintf(f, "aim target - %f %f %f\n", min2.x, min2.y, min2.z); fflush(f);
								Aim(pLocalPlayer, min2);
							}
					
					}
				}
			}

		}
	}
	
}