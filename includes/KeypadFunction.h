#ifndef KEYPADFUNCTION_H_
#define KEYPADFUNCTION_H_

void keyboardFunction(void)
{
	digitBlinkFlag = 1;
	uint8_t digitPlace = 0;
	char passwordToEnter[5]={0x30,0x30,0x30,0x30,'\0'};
	flagWritten=0;
	sendToLcd(COMMAND, WRITE, 0x01);
	sendToLcd(COMMAND,WRITE,0x80);
	sendStringToLcd("PASSWORD");
	sendToLcd(COMMAND, WRITE, 0xC0);
	//sendStringToLcd("0000");
	sendStringToLcd(passwordToEnter);
	//int i=1;
	_delay_ms(1000);
	
	
	while (1)
	{
		if ((digitBlinkCount % 2) == 0)
		{
			sendToLcd(COMMAND,WRITE,(0xC0 + digitPlace));
			sendStringToLcd(" ");
		}
		
		else
		{
			sendToLcd(COMMAND,WRITE,0xC0);
			sendStringToLcd(passwordToEnter);
		}
		
		if ((READBIT(PINA,UP)) == 0)
		{
			_delay_ms(DEBOUNCE_VALUE);
			if ((READBIT(PINA,UP)) == 0)
			{
				if ((passwordToEnter[digitPlace]) < (9 + 0x30))
				{
					passwordToEnter[digitPlace] += 1;
				}
				
				sendToLcd(COMMAND,WRITE,0xC0);
				sendStringToLcd(passwordToEnter);
			}
		}
		
		if ((READBIT(PINA,DOWN)) == 0)
		{
			_delay_ms(DEBOUNCE_VALUE);
			if ((READBIT(PINA,DOWN)) == 0)
			{
				if (passwordToEnter[digitPlace] > (0x30))
				{
					passwordToEnter[digitPlace] -= 1;
				}
				
				sendToLcd(COMMAND, WRITE, 0xC0);
				sendStringToLcd(passwordToEnter);
			}
		}
		if ((READBIT(PINA,SET)) == 0 )
		{
			_delay_ms(DEBOUNCE_VALUE);
			if (READBIT(PINA,SET) == 0)
			{
				while((READBIT(PINA,SET)) == 0);
				if (digitPlace < 3)
				{
					digitPlace += 1;
				}
				
				else
				{
					password = 1000*(passwordToEnter[0] - 0X30) + 100*(passwordToEnter[1] - 0X30) +
					10*(passwordToEnter[2]  - 0X30) + (passwordToEnter[3] - 0X30);
					if (password == presetPassword)
					{
						sendToLcd(COMMAND,WRITE,0x01);
						sendToLcd(COMMAND,WRITE,0x80);
						//-----------------------------------------------------PASSWORD DONE----------------------------------------------
						sendStringToLcd("WELCOME!..");
						_delay_ms(500);
						//sendToLcd(COMMAND,WRITE,0x01);
						sendToLcd(COMMAND,WRITE,0X80);
						sendStringToLcd("SET LINE PRESSURE:");
						sendToLcd(COMMAND,WRITE,0XC0);
						convertAndSend(defaultPressure);
						sendToLcd(COMMAND,WRITE,0xC6);
						sendStringToLcd("Bar");
						digitPlace = 0;
						//defPresSet = defaultPressure;
						while(1)
						{
							if ((digitBlinkCount % 2) == 0)
							{
								if(digitPlace!=2) sendToLcd(COMMAND,WRITE,(0xC0 + digitPlace));
								else sendToLcd(COMMAND,WRITE,(0xC0 + digitPlace + 1));
								sendStringToLcd(" ");
							}
							
							else
							{
								sendToLcd(COMMAND,WRITE,0xC0);
								convertAndSend(defPresSet);
							}
							if ((READBIT(PINA,UP)) == 0)
							{
								_delay_ms(DEBOUNCE_VALUE);
								if ((READBIT(PINA,UP)) == 0)
								{
									switch (digitPlace)
									{
										case 0:
										if(defPresSet==0) defPresSet += 10;
										break;
										
										case 1:
										if (defPresSet <= 9)
										{
											defPresSet += 1;
										}
										break;
										
										case 2:
										if (defPresSet < 10)
										{
											defPresSet += 0.1;
										}
										break;
									}
									sendToLcd(COMMAND,WRITE,0xC0);
									convertAndSend(defPresSet);
								}
							}
							
							if ((READBIT(PINA,DOWN)) == 0)
							{
								_delay_ms(DEBOUNCE_VALUE);
								if ((READBIT(PINA,DOWN)) == 0)
								{
									switch (digitPlace)
									{
										case 0:
										if(defPresSet==10) defPresSet -= 10;
										break;
										
										case 1:
										if (defPresSet > 0)
										{
											defPresSet -= 1;
										}
										break;
										
										case 2:
										if (defPresSet > 0.1)
										{
											defPresSet -= 0.1;
										}
										break;
									}
									
									sendToLcd(COMMAND,WRITE,0xC0);
									convertAndSend(defPresSet);
								}
							}
							
							if ((READBIT(PINA,SET)) == 0)
							{
								_delay_ms(DEBOUNCE_VALUE);
								if ((READBIT(PINA,SET)) == 0)
								{
									while((READBIT(PINA,SET)) == 0);
									if (digitPlace < 2)
									{
										digitPlace += 1;
									}
									//-----------------------------------------------------------LINE PRESSURE SET------------------------------------------------------------------------------------
									else
									{
										defaultPressure = defPresSet;
										sendToLcd(COMMAND,WRITE,0x01);
										sendToLcd(COMMAND,WRITE,0X80); //sTART AT FIRST LINE
										sendStringToLcd("SET CUTOFF PRESSURE");
										cli();
										eeprom_update_float(&eeDefaultPres,defPresSet);
										sei();
										digitPlace = 0;
										sendToLcd(COMMAND,WRITE,0XC0);
										convertAndSend(minPressure);
										sendToLcd(COMMAND,WRITE,0xC6);
										sendStringToLcd("Bar");
										minPresSet = minPressure;
										
										while(1)
										{
											if ((digitBlinkCount % 2) == 0)
											{
												if(digitPlace!=2) sendToLcd(COMMAND,WRITE,(0xC0 + digitPlace));
												else sendToLcd(COMMAND,WRITE,(0xC0 + digitPlace + 1));
												sendStringToLcd(" ");
											}
											
											else
											{
												sendToLcd(COMMAND,WRITE,0xC0);
												convertAndSend(minPresSet);
											}
											if ((READBIT(PINA,UP)) == 0)
											{
												_delay_ms(DEBOUNCE_VALUE);
												if ((READBIT(PINA,UP)) == 0)
												{
													switch (digitPlace)
													{
														case 0:
														if(minPresSet==0) minPresSet += 10;
														break;
														
														case 1:
														if (minPresSet < (defPresSet-1))
														{
															minPresSet += 1;
														}
														break;
														
														case 2:
														if (minPresSet < (defPresSet))
														{
															minPresSet += 0.1;
														}
														break;
													}
													sendToLcd(COMMAND,WRITE,0xC0);
													convertAndSend(minPresSet);
												}
											}
											
											if ((READBIT(PINA,DOWN)) == 0)
											{
												_delay_ms(DEBOUNCE_VALUE);
												if ((READBIT(PINA,DOWN)) == 0)
												{
													switch (digitPlace)
													{
														case 0:
														//if(defPresSet==10) defPresSet -= 10;
														break;
														
														case 1:
														if (minPresSet >= 1)
														{
															minPresSet -= 1;
														}
														break;
														
														case 2:
														if (minPresSet > 0.1)
														{
															minPresSet -= 0.1;
														}
														break;
													}
													
													sendToLcd(COMMAND,WRITE,0xC0);
													convertAndSend(minPresSet);
												}
											}
											
											if ((READBIT(PINA,SET)) == 0)
											{
												_delay_ms(DEBOUNCE_VALUE);
												if ((READBIT(PINA,SET)) == 0)
												{
													while((READBIT(PINA,SET)) == 0);
													if (digitPlace < 2)
													{
														digitPlace += 1;
													}
													//-----------------------------------------------------------CUTOFF PRESSURE SET//DELAY TIMER SET START------------------------------------------------------------------------------------
													else
													{
														minPressure = minPresSet;
														sendToLcd(COMMAND,WRITE,0x01);
														sendToLcd(COMMAND,WRITE,0X80); //sTART AT FIRST LINE
														sendStringToLcd("SET TIMER DELAY");
														cli();
														eeprom_update_float(&eeMinPres,minPresSet);
														sei();
														digitPlace = 0;
														sendToLcd(COMMAND,WRITE,0XC0);
														char delayTimer[5] = {((timerValue/1000)+(0x30)),(((char)(timerValue/100)%10)+0x30),(((char)(timerValue/10)%10)+0x30),(((char)(timerValue)%10)+0x30),'\0'};
														sendStringToLcd(delayTimer);
														sendToLcd(COMMAND,WRITE,0xC6);
														sendStringToLcd("SEC");
														
														while(1)
														{
															if ((digitBlinkCount % 2) == 0)
																{
																	sendToLcd(COMMAND,WRITE,(0xC0 + digitPlace));
																	sendStringToLcd(" ");
																}
																
															else
																{
																	sendToLcd(COMMAND,WRITE,0xC0);
																	sendStringToLcd(delayTimer);
																}
															
															if ((READBIT(PINA,UP)) == 0)
															{
																_delay_ms(DEBOUNCE_VALUE);
																if ((READBIT(PINA,UP)) == 0)
																{
																	if ((delayTimer[digitPlace]) < (9 + 0x30))
																	{
																		delayTimer[digitPlace] += 1;
																	}
																	
																	sendToLcd(COMMAND,WRITE,0xC0);
																	sendStringToLcd(delayTimer);
																}
															}
															
															if ((READBIT(PINA,DOWN)) == 0)
															{
																_delay_ms(DEBOUNCE_VALUE);
																if ((READBIT(PINA,DOWN)) == 0)
																{
																	if (delayTimer[digitPlace] > (0x30))
																	{
																		delayTimer[digitPlace] -= 1;
																	}
																	
																	sendToLcd(COMMAND, WRITE, 0xC0);
																	sendStringToLcd(delayTimer);
																}
															}
															if ((READBIT(PINA,SET)) == 0 )
															{
																_delay_ms(DEBOUNCE_VALUE);
																if ((READBIT(PINA,SET)) == 0)
																{
																	while ((READBIT(PINA,SET)) == 0);
																	if (digitPlace < 3)
																	{
																		digitPlace += 1;
																	}
																	
																	else
																	{
																		timerValue = 1000*(delayTimer[0] - 0X30) + 100*(delayTimer[1] - 0X30) +
																		10*(delayTimer[2]  - 0X30) + (delayTimer[3] - 0X30);
																		cli();
																		eeprom_update_word(&eeTimerDelay,timerValue);
																		_delay_ms(10);
																		sei();
																		_delay_ms(2);
																		sendToLcd(COMMAND,WRITE,0x01);
																		sendToLcd(COMMAND,WRITE,0x80);
																		sendStringToLcd("SET TIMINGS:");
																		displayTime(startHour,0xC0);
																		sendStringToLcd(":");
																		displayTime(startMinute,0xC3);
																		sendToLcd(COMMAND,WRITE,0xC7);
																		sendStringToLcd("TO");
																		displayTime(endHour,0xCB);
																		sendToLcd(COMMAND,WRITE,0xCD);
																		sendStringToLcd(":");
																		digitPlace = 0;
																		displayTime(endMinute,0xCE);
																		
																		while(1)
																		{
																			if ((READBIT(PINA,UP)) == 0)
																			{
																				_delay_ms(DEBOUNCE_VALUE);
																				if ((READBIT(PINA,UP)) == 0)
																				{
																					switch(digitPlace)
																					{
																						case 0:
																						if (startMinute < 45 && startHour < 24)
																						{
																							startMinute += 15;
																						}
																						else
																						{
																							startMinute = 0;
																							if (startHour<=23)
																							{
																								startHour+=1;
																							}
																						}
																						displayTime(startHour,0xC0);
																						displayTime(startMinute,0xC3);
																						break;
																		//---------------------------endHour/min............//				
																						case 1:
																						if (endMinute < 45)
																						{
																							endMinute += 15;
																						}
																						else
																						{
																							endMinute = 0;
																							if (endHour<=23)
																							{
																								endHour+=1;
																							}
																						}
																						displayTime(endHour,0xCB);
																						displayTime(endMinute,0xCE);
																						break;
																					}
																				}
																			}
																			
																			if ((READBIT(PINA,DOWN)) == 0)
																			{
																				_delay_ms(DEBOUNCE_VALUE);
																				if ((READBIT(PINA,DOWN)) == 0)
																				{
																					switch(digitPlace)
																					{
																						case 0:
																						if (startMinute >= 15)
																						{
																							startMinute -= 15;
																						}
																																											
																						else if (startMinute == 0 && startHour>0)
																						{
																							
																							startMinute = 45;
																							startHour -= 1;
																							
																						}
																						displayTime(startHour,0xC0);
																						displayTime(startMinute,0xC3);
																						break;
																						//---------------------------endHour/min............//
																						case 1:
																						if (endMinute >= 15)
																						{
																							endMinute -= 15;
																						}
																						
																					
																						
																						else if (endMinute == 0 && endHour>0)
																						{
																							endMinute = 45;
																							endHour-=1;
																						}
																						displayTime(endHour,0xCB);
																						displayTime(endMinute,0xCE);
																						break;
																					}
																				}
																			}
																			
																			if ((READBIT(PINA,SET)) == 0)
																			{
																				_delay_ms(DEBOUNCE_VALUE);
																				if ((READBIT(PINA,SET)) == 0)
																				{
																					while ((READBIT(PINA,SET)) == 0);
																					if (digitPlace < 1)
																					{
																						digitPlace += 1;
																					}
																					
																					else
																					{
																						cli();
																						eeprom_update_byte(&eeStartHour,startHour);
																						eeprom_update_byte(&eeStartMinute,startMinute);
																						eeprom_update_byte(&eeEndHour,endHour);
																						eeprom_update_byte(&eeEndMinute,endMinute);
																						sei();
																						digitPlace = 0;
																						digitBlinkFlag = 0;
																						return;
																					}
																				}
																			}
																		}																	
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
						
					}
					else
					{
						sendToLcd(COMMAND,WRITE,0x01);
						sendToLcd(COMMAND,WRITE,0x80);
						
						sendStringToLcd("WRONG PASSWORD");
						_delay_ms(500);
						digitBlinkFlag = 0;
						return;
					}
					
					
				}
				
			}
		}
	}
}

//
//if ((READBIT(PINA,UP)) == 0)
//{
//
//
//SendDigitsToLcd(i,0xC0);
//
//
//}
//
//if ((READBIT(PINA,DOWN)) == 0)
//{
//i-=((10^digitPlace));
//password=i;
//if (i<0)
//{
//i=9999;
//password = i;
//}
//SendDigitsToLcd(i,0xC0);
//
//
//}
//
//if ((READBIT(PINA,ENTER)) == 0) //Proceed further to settings
//{
//if (password==9999)
//{
//CLEARBIT(PORTB,PUMP);
//pumpOnFlag = 0;
//sendToLcd(COMMAND,WRITE,0x01);
//sendStringToLcd("Welcome");
//_delay_ms(50);
//sendToLcd(COMMAND, WRITE, 0x01);
//sendStringToLcd("SET DEFAULT PRESSURE:");
//sendToLcd(COMMAND, WRITE, 0xC0);
//
//convertAndSend(defPresSet);
//sendStringToLcd(" Bar");
//while (1) //Set default Pressure
//{
//
//if ((READBIT(PINA,UP)) == 0)
//{
//if (defPresSet < 9.9)
//{
//defPresSet += 0.1;
//}
//
//sendToLcd(COMMAND, WRITE, 0xC0);
//convertAndSend(defPresSet);
//
//}
//
//if ((READBIT(PINA,DOWN)) == 0)
//{
//if(defPresSet!=0) defPresSet -= 0.1;
//sendToLcd(COMMAND, WRITE, 0xC0);
//convertAndSend(defPresSet);
//}
//
//if ((READBIT(PINA,ENTER)) == 0)
//{
////eeprom_update_float(&eeDefPres, defPresSet);
//sendToLcd(COMMAND, WRITE, 0x01);
//sendStringToLcd("SET MIN PRESSURE");
//sendToLcd(COMMAND, WRITE, 0xC0);
//convertAndSend(minPresSet);
//sendStringToLcd(" Bar");
//while(1) //set Min Pressure
//{
//if ((READBIT(PINA,UP)) == 0)
//{
//if (minPresSet <= (defPresSet)) minPresSet += 0.1;
//sendToLcd(COMMAND, WRITE, 0xC0);
//convertAndSend(minPresSet);
//}
//
//if ((READBIT(PINA,DOWN)) == 0)
//{
//if(minPresSet>0) minPresSet -= 0.1;
//sendToLcd(COMMAND, WRITE, 0xC0);
//convertAndSend(minPresSet);
//}
//
//if ((READBIT(PINA,ENTER)) == 0)
//{
//
////eeprom_update_float(&eeMinPres, minPresSet);
//sendToLcd(COMMAND, WRITE, 0x01);
//sendStringToLcd("LONG DELAY TIMER:");
//sendToLcd(COMMAND,WRITE,0xC6);
//sendStringToLcd("Sec");
//SendDigitsToLcd(timerValue,0xC0);
//int digitPlace =1;
//while(1)
//{
//
//if ((READBIT(PINA,UP)) == 0)
//{
//timerValue+=digitPlace;
//if (timerValue>9999) timerValue=0;
//SendDigitsToLcd(timerValue,0xC0);
//}
//
//if ((READBIT(PINA,DOWN)) == 0)
//{
//timerValue-=digitPlace;
//if (timerValue<0) timerValue=9999;
//SendDigitsToLcd(timerValue,0xC0);
//}
//
//
//if (READBIT(PINA,MENU))
//{
//if (digitPlace!=1000)
//{
//digitPlace*=10;
//}
//
//else digitPlace=1;
//switch(digitPlace)
//{
//case 1:
//sendToLcd(COMMAND,WRITE,0xC3);
//sendStringToLcd(" ");
//break;
//
//case 10:
//sendToLcd(COMMAND,WRITE,0xC2);
//sendStringToLcd(" ");
//break;
//
//case 100:
//sendToLcd(COMMAND,WRITE,0xC1);
//sendStringToLcd(" ");
//break;
//
//case 1000:
//sendToLcd(COMMAND,WRITE,0xC0);
//sendStringToLcd(" ");
//break;
//}
//}
//
//if ((READBIT(PINA,ENTER)) == 0)
//{
//minPressure = minPresSet;
//defaultPressure = defPresSet;
//
//
//eeprom_update_float(&eeMinPres,minPressure);
//eeprom_update_float(&eeDefPres,defaultPressure);
//return;
//}
//
//}
//}
//
//}
//}
//}
//
//}
//else
//{
//sendToLcd(COMMAND, WRITE, 0x01);
//sendToLcd(COMMAND, WRITE, 0x80);
//sendStringToLcd("Wrong Password");
//_delay_ms(100);
//return;
//}
//
//
//}
//Commented Section



#endif /* KEYPADFUNCTION_H_ */