/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }
  
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOn(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
  
  LedOn(LCD_RED);
  LedOn(LCD_GREEN);
  LedOn(LCD_BLUE);
  
  LcdCommand(LCD_CLEAR_CMD);
  

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
    //Password stored in array, 1 = Button 0, 2 = Button 1, 3 = Button 2 
    u8 au8Code[10] = {1,2,3,2,1,0,0,0,0,0};
    u8 u8Flag = 0;
    
    u8 au8EnterCode[] = "Enter Password";
    u8 au8Message[] = "INCORRECT";
    
    //User Input stored in input array
    static u8 au8Input[10] = {0,0,0,0,0,0,0,0,0,0}; 
    
    //Counter of position in input array
    static u8 u8Position = 0;
    
    //Prints message to enter password
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR, au8EnterCode);
    
    
    //resets position if goes outside array bounds;
    if(u8Position == 10)
    {
      u8Position = 0;
      u8Flag = 1;
    }

    
    
    //Checks what button is pressed and adds to the input array
    if( WasButtonPressed(BUTTON0))
    {
      ButtonAcknowledge(BUTTON0);
      LedToggle(YELLOW);
      LedOff(RED);
      LedOff(GREEN);
      
      au8Input[u8Position] = 1;
      u8Position++;
    }
    else if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      LedToggle(YELLOW);
      LedOff(RED);
      LedOff(GREEN);
      
      au8Input[u8Position] = 2;
      u8Position++;
    }
    else if(WasButtonPressed(BUTTON2))
    {
      ButtonAcknowledge(BUTTON2);
      LedToggle(YELLOW);
      LedOff(RED);
      LedOff(GREEN);
      
      au8Input[u8Position] = 3;
      u8Position++;
    }
    
    //When Button 3 is pressed checks if arrays match
    
    if(WasButtonPressed(BUTTON3))
    {
      ButtonAcknowledge(BUTTON3);
      
      u8Position = 0;
      LedOff(YELLOW);
      LedOff(RED);
      LedOff(GREEN);
      
      for(u8 i = 0; i < 10; i++)
      {
        if(au8Input[i] != au8Code[i])
        {
          u8Flag = 1;
        }
      }
      
      
      //Turn on LEDs and change the message to be displayed
      if(u8Flag == 1)
      {
        LedOn(RED);
        au8Message = "INCORRECT";
 
        u8Flag = 0;
      }
      else
      {
        LedOn(GREEN);
        au8Message = "CORRECT  ";
      }
      
      //Clears Input Array
      for(u8 i = 0; i < 10; i++)
      {
        au8Input[i] = 0;
      }
      //Displays message
       LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, au8Message);
      
      //Switch to state that waits for Button 3 Press
      UserApp1_pfStateMachine = UserApp1SM_WaitForPress;
    }
} /* end UserApp1SM_Idle() */
     

static void UserApp1SM_WaitForPress(void)
{
  
  static u8 u8Timer = 0;
  u8 au8PressB3[] = "Press Button 3 to retry";
  
  u8Timer++;
  
  //Prints message to press button 3 to continue after 4s
  if(u8Timer == 4000)
  {
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR, au8PressB3); 
  }
  
  if(WasButtonPressed(BUTTON3))
    {
      ButtonAcknowledge(BUTTON3);
      UserApp1_pfStateMachine = UserApp1SM_Idle;
      LedOn(YELLOW);
      LedOff(GREEN);
      LedOff(RED);
      ButtonAcknowledge(BUTTON2);
      ButtonAcknowledge(BUTTON1);
      ButtonAcknowledge(BUTTON0);
    }   
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
