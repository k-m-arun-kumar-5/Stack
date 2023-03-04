/* ======================================================================
  File Name     : Stack.C
  Description   : stack is implemented using array with variable data type in a stack node.
  Author        : K.M. Arun Kumar alias Arunkumar Murugeswaran
  Date          :
  Remarks     1 : by default, global execution flow flag is disabled for debugging.
              2 : stack, is implemented with array.
  Known Bugs  1 : giving non numeric input for numeric data.
  Modification
       History  :
  Others        :
====================================================================== */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define TREE_NODE_MSG          3  /* stack data has sample's tree node's addr */
#define EMP_MSG                1  /* stack's data has Employee's record */
#define NUM_MSG                2  /* tree node's data has sample alpha data */
#define ALPHA_DATALEN          (sizeof(char))
#define TRUE                   0
#define FALSE                 -1
#define NO_MEMORY             -2
#define MAX_TREE_NODES        15  /* max number of nodes in tree from tree's root */
#define MAX_TREE_DATA         20  /* max data size does queue & tree can have */
#define QUIT_OPER              1
#define PUSH_DATA_OPER         2
#define POP_DATA_OPER          3
#define DESTROY_STACK_OPER     4  /* destroy stack */
#define DISPLAY_OPER           5
#define DESTROY_STACK_ELEMENTS 6
#define RETRIEVE_TOP           1
#define RETRIEVE_WHOLE_STACK   2
#define STACK_MEGA_HEAD        3
#define RETRIEVE_NODE          (DISPLAY_OPER + 1)  /* display stack node's data */
#define MAX_STACK_SIZE        20  /* maximum no of stack nodes */
#define OPER_SIZE             15
#define MAX_NAME              (MAX_TREE_DATA - sizeof(unsigned) - sizeof(unsigned char))
#define NO_ACCESS              1  /* does not allow access to queue */
#define FREE_ACCESS            0  /* allow access to queue */
#define LEFT_NODE              1  /* indicates left node */
#define RIGHT_NODE             2  /* indicates right node */
#define INVALID_DATA         255  /* invalid data that tree node can have */
#define TRACE_ON               1  /* enable global to trace execution flow for debugging */
#define TRACE_OFF              0  /* disable global to trace execution flow for debugging */

typedef struct
{
	unsigned char msgtype;      /* differentiate kind of data in stack */
	unsigned char datalen;      /* variable data's length */
	void *databuff;             /* contains variable data */
} stack_data_t;

typedef struct stacknode
{
	void *databuff;
	struct stacknode *next_node_ptr;
} stack_node;

typedef struct
{
   unsigned char stack_count;       /* number of nodes in stack */
   unsigned char max_stacksize;     /* Max number of allowable nodes in stack */
   int top_index;                   /* holds stack's top index, where new data will be push. top index - 1 has data, to be poped */
   char access_stack;               /* control access to stack */
   unsigned char max_datasize;      /* max memory size req for data in stack node */
   stack_node *top_stack_node;
   stack_node *stack_array;         /* starting address of stack implemented by 1D array */
} stack_head;

typedef struct
{
	unsigned emp_id;              /* Employee ID, used as key */
	unsigned char name_len;       /* Total length of Emp's Name */
	void  *databuff;              /* variable Employee Name */
} emp_info;



/* global variable declaration */
char trace_flag = TRACE_OFF;
stack_head stack_megahead;

/* function prototype */
int Create_Stack_Pool(stack_head *stack_megaptr, unsigned max_nodes, unsigned max_datasize);
int Initialize_Stack(unsigned char mode, stack_head *stack_megaptr);
int Control_Stack(stack_head *stack_megaptr, char access_mode);
int Empty_Stack(stack_head *stack_megaptr);
int Full_Stack(stack_head *stack_megaptr);
int Access_Stack(stack_head *stack_megaptr);
int Stack_Count(stack_head *stack_megaptr);
int Display_Data(void);
int Display_Stack(int mode, stack_head *stack_megaptr);
int Delete_Stack_Emp_Data(stack_head *stack_megaptr, void *delete_data_ptr );
int Push_Data(stack_head *stack_megaptr, unsigned char data_len, void *push_data);
int Push_EmpData(stack_head *stack_megaptr, stack_data_t *push_emp_ptr);
int Pop_Data(stack_head *stack_megaptr, unsigned char *stack_datalen, void **const pop_data);
int Stack_Emp_Data(unsigned char mode, stack_head *stack_megaptr, void *pop_data);
int Retrieve_StackData(int ret_mode, stack_head *stack_megaptr, stack_node *cur_node, unsigned char *stack_datalen, void *const *pop_data);
int Destroy_Stack(unsigned char mode, stack_head *stack_megaptr);
int GetStack_Data(unsigned get_mode, stack_head *stack_megaptr,unsigned char *stack_datalen, stack_data_t **pop_data );
int StackData_Range(unsigned mode, stack_head *stack_megaptr);
int Stack_Sub_Data(unsigned char mode, stack_head *stack_megaptr, void *data_ptr);
int Push_NumData(stack_head *stack_megaptr, stack_data_t *push_num_ptr);
int Push_Data_Msg(stack_head *stack_megaptr);
int Delete_Stack_Num_Data(void *delete_data_ptr);
int Stack_Num_Data(unsigned char mode, stack_head *stack_megaptr, void *pop_data);

/*******************************************************************
 Function Name  : main()
 Description    : navigation operation on automatically created tree
 Remarks        :
 Func ID        : 1
*******************************************************************/
int main()
{
    int nav_mode = QUIT_OPER;
    unsigned char stack_datalen;
    stack_data_t *pop_data = NULL;
    int ret_state;
    emp_info *pop_emp = NULL, get_emp ;
	stack_data_t *stack_data_ptr;
	char *ptr;

     Initialize_Stack(DESTROY_STACK_OPER, &stack_megahead);
     if((Create_Stack_Pool(&stack_megahead, MAX_STACK_SIZE, MAX_TREE_DATA )) != TRUE)
	     return FALSE;
    do
    {
       printf("\n MENU: 1:Quit, 2:Push, 3:Pop, 4:Destroy S, 5:Display: Enter: ");
       scanf("%d", &nav_mode);
       switch(nav_mode)
       {
		   case PUSH_DATA_OPER:
		     if((Push_Data_Msg(&stack_megahead)) != TRUE)
		        continue;
		     break;
		   case POP_DATA_OPER:
		     if((Pop_Data(&stack_megahead, &stack_datalen, (void **)&pop_data)) != TRUE )
		        continue;
			 if((Stack_Sub_Data(nav_mode, &stack_megahead, (void *)pop_data)) != TRUE)
				 continue;
			 break;
		   case DESTROY_STACK_OPER:
		     if(StackData_Range(DESTROY_STACK_ELEMENTS, &stack_megahead) != TRUE)
		       continue;
		   break;
           case DISPLAY_OPER:
		     Display_Data();
		     break;
		   case QUIT_OPER:
		     /* destroy stack & tree, before quit */
		     StackData_Range(DESTROY_STACK_OPER, &stack_megahead );
		     break;
		   default:
		     printf("\n ERR[1.3]: Invalid navigation oper: %d", nav_mode);
       }
    } while (nav_mode != QUIT_OPER);
    return TRUE;
}

/*******************************************************************
 Function Name  : Display_Data(void *disp_data)
 Description    : gets display mode, and display related data
 Remarks        :
 Func ID        : 2
*******************************************************************/
 int Display_Data(void)
 {
 	 int display_mode;
     int ret_state = TRUE;
     stack_head *stack_megaptr = &stack_megahead;

     printf("\n Stack  - 1: Top,  2: Whole,  3: S Meta Data ");
     printf("\n Enter Display Mode: ");
     scanf("%d", &display_mode);
     switch(display_mode)
     {
 		case STACK_MEGA_HEAD:
        case RETRIEVE_TOP:
 	    case RETRIEVE_NODE:
		case RETRIEVE_WHOLE_STACK:
 	      ret_state = Display_Stack(display_mode, stack_megaptr);
 	      break;
 		default:
           printf("\n ERR[2.2]: Invalid display mode: %d",display_mode );
           ret_state = FALSE;
     }
     return ret_state;
}

/*******************************************************************
 Function Name  : Create_Stack_Pool(stack_head *stack_megaptr, int max_nodes, unsigned max_datasize)
 Description    : initialize of stack's mega head node
 Remarks        : assume that input parameter are valid
 Func ID        : 3
 *******************************************************************/
int Create_Stack_Pool(stack_head *stack_megaptr, unsigned max_nodes, unsigned max_datasize )
{
	if(stack_megaptr == NULL)
	{
		printf("\n ERR[3.1]: Invalid stack megahead address to initialize");
		return FALSE;
    }
    if(!(stack_megaptr->stack_array = (stack_node *) calloc(max_nodes, sizeof(stack_node))))
      return NO_MEMORY;
    stack_megaptr->stack_count = 0;
    stack_megaptr->max_stacksize = max_nodes;
    /* max data size that quedatabuff, will contain */
    stack_megaptr->max_datasize = max_datasize;
    stack_megaptr->top_index = 0;
    stack_megaptr->top_stack_node = NULL;
    Control_Stack(stack_megaptr,FREE_ACCESS );
    if(trace_flag)
       printf("\n TRACE[3.2]:Initialized S's head: %#X, S array Addr: %#X , Max S elements: %u, S's element data_size: %u",\
        stack_megaptr,stack_megaptr->stack_array,stack_megaptr->max_stacksize, stack_megaptr->max_datasize);
    return TRUE;
}

/*******************************************************************
 Function Name  : Initialize_Stack
 Description    : initialize of stack's mega head node
 Remarks        : assume that input parameter are valid
 Func ID        : 4
 *******************************************************************/
int Initialize_Stack(unsigned char mode, stack_head *stack_megaptr)
{
	if(stack_megaptr == NULL)
	{
		printf("\n ERR[4.1]: Invalid stack megahead address to initialize");
		return FALSE;
    }
	if(mode == DESTROY_STACK_OPER)
	{
        /* free dynamic 1D array */
 	    if(stack_megaptr->stack_array)
 	        free(stack_megaptr->stack_array);
 	    /* initialize with invalid data */
 	    stack_megaptr->stack_array = NULL;
	    stack_megaptr->max_stacksize = 0;
        stack_megaptr->max_datasize = 0;
	}
	stack_megaptr->top_stack_node = NULL;
 	stack_megaptr->stack_count = 0;
    stack_megaptr->top_index = 0;
    return TRUE;
}
/*******************************************************************
 Function Name : Control_Stack(stack_head *stack_megaptr, int access_mode)
 Description   : control access of the stack
 Remarks       : useful for multi operation be performed in it
 Func ID       : 5
 *******************************************************************/
int Control_Stack(stack_head *stack_megaptr, char access_mode)
{
  	int istrue = FALSE;

  	if(stack_megaptr == NULL)
 	{
 		printf("\n ERR[5.1]: Invalid S head's address to control its access ");
 		return istrue;
     }
  	switch(access_mode)
  	{
  		case FREE_ACCESS:
  		case NO_ACCESS:
  		  stack_megaptr->access_stack = access_mode;
  		  istrue = TRUE;
  		  break;
  		default:
  		  printf("\n ERR[5.2]: Invalid S's Addr: %#X access mode: %d", stack_megaptr,access_mode);
     }
     return istrue;
 }
/*******************************************************************
 Function Name  : Empty_Stack(stack_head *stack_megaptr)
 Description    : checks for empty stack
 Remarks        :
 Func ID        : 6
 *******************************************************************/
 int Empty_Stack(stack_head *stack_megaptr)
 {
	 int ret_state = FALSE;

     if((Access_Stack(stack_megaptr)) == TRUE)
	 {
		Control_Stack(stack_megaptr, NO_ACCESS);
		if(stack_megaptr->stack_count <= 0)
		{
		   printf("\n ERR[6.1]: Empty Stack");
		   ret_state = TRUE;
		}
	    Control_Stack(stack_megaptr, FREE_ACCESS);
	 }
     return ret_state;
 }

 /*******************************************************************
  Function Name  : Full_Stack(stack_head *stack_megaptr)
  Description    : checks for stack been full
  Remarks        : also checks availability memory
  Func ID        : 7
  *******************************************************************/
 int Full_Stack(stack_head *stack_megaptr)
 {
	 int ret_state = FALSE;
     void *check_alloc = NULL;

	 if((Access_Stack(stack_megaptr)) == TRUE)
	 {
		 Control_Stack(stack_megaptr, NO_ACCESS);
		 if(stack_megaptr->stack_count >= stack_megaptr->max_stacksize )
		 {
			  printf("\n ERR[7.1]: no of nodes [%u] exceeds its max [%u]", stack_megaptr,stack_megaptr->stack_count, stack_megaptr->max_stacksize);
			  ret_state = TRUE;
	     }
	     Control_Stack(stack_megaptr, FREE_ACCESS);
	 }
	 return ret_state;
 }

/*******************************************************************
 Function Name  : Access_Stack(stack_head *stack_megaptr)
 Description    : check for stack access
 Remarks        :
 Func ID        : 8
 *******************************************************************/
int Access_Stack(stack_head *stack_megaptr)
{
   if(stack_megaptr == NULL)
   {
   	  printf("\n ERR[8.1]: Invalid S head's address to access");
   	  return FALSE;
   }
   if(stack_megaptr->access_stack != FREE_ACCESS)
   {
      /* printf("\n ERR[8.2]: Unable for S's Head Addr: %#X, to access: %d",stack_megaptr,stack_megaptr->access_stack );*/
       return FALSE;
   }
   return TRUE;
}

/*******************************************************************
  Function Name  : Stack_Count(stack_head *stack_megaptr )
  Description    : number of nodes in the stack
  Remarks        :
  Func ID        : 9
 *******************************************************************/
 int Stack_Count(stack_head *stack_megaptr)
 {
	if((Access_Stack(stack_megaptr)) == TRUE)
 	{
 	   if(trace_flag)
    	   printf("\n TRACE[9.1]: S Head's Addr: %#X, current num of stack nodes: %u",stack_megaptr, stack_megaptr->stack_count);
 	    return stack_megaptr->stack_count;
    }
    return FALSE;
 }

/*******************************************************************
 Function Name  : Push_Data(stack_head *stack_megaptr, stack_node *push_data )
 Description    : insert data provided through its address at top of stack
 Remarks        :
 Func ID        : 10
 *******************************************************************/
int Push_Data(stack_head *stack_megaptr, unsigned char data_len, void *const push_data)
{
	stack_node *temp;

    if((Access_Stack(stack_megaptr)) == TRUE)
	{
	   /* total size that stack's data buffer requires should not exceeds the limit */
	   if((((Full_Stack(stack_megaptr)) == TRUE)) || data_len > stack_megaptr->max_datasize)
	   {
		  Control_Stack(stack_megaptr, NO_ACCESS);
	      printf("\n ERR[10.1]: Try to Push S's Top: %d, datalen [%u] > max [%u]",stack_megaptr->top_index, data_len, stack_megaptr->max_datasize);
		  Control_Stack(stack_megaptr, FREE_ACCESS);
	      return FALSE;
       }
	    stack_megaptr->stack_array[stack_megaptr->top_index].databuff = push_data;
	    if(trace_flag)
        {
              printf("\n TRACE[10.2]: stack_top: %d, Push Node: %#X", \
              stack_megaptr->top_index, stack_megaptr->stack_array[stack_megaptr->top_index].databuff );
        }
    	 stack_megaptr->top_stack_node = (stack_megaptr->stack_array + stack_megaptr->top_index);
         if(stack_megaptr->top_index)
		 {
			  temp = (stack_megaptr->stack_array + stack_megaptr->top_index);
		     (stack_megaptr->stack_array + (stack_megaptr->top_index + 1))->next_node_ptr = temp;
		 }
		 else
			(stack_megaptr->stack_array + stack_megaptr->top_index)->next_node_ptr = NULL;
		 ++stack_megaptr->top_index;
		 ++stack_megaptr->stack_count;
         Control_Stack(stack_megaptr, FREE_ACCESS);
         return TRUE;
    }
    return FALSE;
}

/*******************************************************************
   Function Name : Push_EmpData(stack_head *stack_megaptr)
   Description   : manually getting a valid input data before push operation
   Remarks       : assume that input data are valid
   fUNC_ID       : 11
 *******************************************************************/
int Push_EmpData(stack_head *stack_megaptr, stack_data_t *push_emp_ptr)
{
    int ret_state = FALSE;
    emp_info *push_emp = NULL;
    stack_data_t *stack_data_ptr;
    unsigned char stackdata_totlen;
    char *name_ptr;
	char name[OPER_SIZE];
	unsigned emp_id, name_len;

	if(push_emp_ptr == NULL)
	{
	    printf("\n Enter Emp ID & its name: ");
        scanf(" %u %[^\n]", &emp_id, name);
       /* name's len also includes NUL character:'\0' */
        name_len = strlen(name) + 1;
       if(name_len > MAX_NAME)
       {
	      printf("\n ERR[11.1]: Entered name %s, whose len %d exceeds tree's data limit: %u",name,name_len,MAX_NAME);
	      return FALSE;
       }
	   if((stack_data_ptr = (stack_data_t *)calloc (1, sizeof(stack_data_t))) == NULL)
	   {
	      printf("\n ERR[11.2]: no memory to allocate");
	      return FALSE;
       }
	   if((push_emp = (emp_info *) calloc(1, sizeof(emp_info))) == NULL)
	   {
	  	 printf("\n Err[11.3]: No memory to alloc ");
	     free(stack_data_ptr);
		 return FALSE;
	   }
	   if((name_ptr = (char *)calloc(1, name_len)) == NULL)
	   {
	       printf("\n Err[11.4]: No memory to alloc ");
		   free(stack_data_ptr);
		   free(push_emp);
		   return FALSE;
	   }
        stack_data_ptr->msgtype = EMP_MSG;
	    stack_data_ptr->datalen = sizeof(emp_info) + name_len - sizeof(void *);
	    stack_data_ptr->databuff = push_emp;
	    push_emp->emp_id = emp_id;
	    push_emp->name_len = name_len;
	    push_emp->databuff = name_ptr;
	    memcpy(name_ptr, name, name_len);
	}
	else
	{
		stack_data_ptr = push_emp_ptr;
	}
	printf("\n Emp Push Data - emp_id: %u, emp name: %s", push_emp->emp_id, (char *)push_emp->databuff);
    if(trace_flag)
	{
	    printf("\n TRACE[11.6]: Push Data - totstack_datalen:%u:: Msgtype:%u", stack_data_ptr->datalen, stack_data_ptr->msgtype);
	    printf("\n TRACE[11.7]: Push emp name Ptr: %#X, Emp Ptr: %#X", push_emp, push_emp->databuff);
	}
	ret_state = Push_Data(stack_megaptr, stack_data_ptr->datalen, stack_data_ptr);
	return ret_state;
}

/*******************************************************************
 Function Name  : Pop_Data(stack_head *stack_megaptr, unsigned char *stack_datalen, void *pop_data)
 Description    : delete node's data provided through its address at top of stack
 Remarks        :
 Func ID        : 12
 *******************************************************************/
 int Pop_Data(stack_head *stack_megaptr, unsigned char *stack_datalen, void **const pop_data)
 {
	 unsigned char pop_datalen;
	 stack_node *pop_node;

     if((Empty_Stack(stack_megaptr)) == TRUE || pop_data == NULL)
     {
		// printf("\n ERR[12.1]: S's Head Addr: %#X is empty to pop", stack_megaptr);
		 *pop_data = NULL;
         return FALSE;
     }
	 if((Access_Stack(stack_megaptr)) == FALSE)
		 return FALSE;
     Control_Stack(stack_megaptr, NO_ACCESS);
	 --stack_megaptr->top_index;
	 pop_node = stack_megaptr->stack_array + stack_megaptr->top_index;
	 if(pop_node == NULL)
	 {
		 printf("\n ERR[12.2]: pop node = NULL");
		 *pop_data = NULL;
		 return FALSE;
	 }
	 *pop_data = pop_node->databuff;
	 stack_megaptr->top_stack_node = (stack_megaptr->stack_array + stack_megaptr->top_index);
     *stack_datalen = ((stack_data_t *)pop_node->databuff)->datalen;
	 --stack_megaptr->stack_count;
	 if(stack_megaptr->top_index >= 0)
	     stack_megaptr->top_stack_node = (stack_megaptr->stack_array + stack_megaptr->top_index);
     if(trace_flag)
     {
	     printf("\n TRACE[12.3]: S's Head Addr: %#X,top_index: %d, Top Node: 0X%X", \
	      stack_megaptr, stack_megaptr->top_index, pop_node );
	     printf("\n TRACE[12.4]: *pop_data Addr: %#X, tot_stackdatalen: %u, Node's stack_databuff: %#X", \
		   *pop_data, *stack_datalen, pop_node->databuff);
     }
     Control_Stack(stack_megaptr, FREE_ACCESS);
	 return TRUE;
 }

/*******************************************************************
   Function Name : Pop_EmpData(stack_node *pop_data)
   Description   : retrive employee info from pop data
   Remarks       : pop_data has info about customer.
   fUNC_ID       : 13
 *******************************************************************/
 int Pop_EmpData(stack_head *stack_megaptr, unsigned char *const emp_len_ptr, void **const pop_data)
 {
	 emp_info *pop_emp = NULL;

	 if( *pop_data == NULL || ((Pop_Data(stack_megaptr, emp_len_ptr, pop_data)) != TRUE))
		 return FALSE;
	 /* decapulation of emp info from stack_data's databuff */
	 if(((stack_data_t *)(*pop_data))->msgtype == EMP_MSG)
	 {
		 pop_emp = ((emp_info *)((stack_data_t *)(*pop_data))->databuff);
		 if(trace_flag)
		 {
			 printf("\n TRACE[13.1]: S data len :%u, emp len: %u", *((unsigned char *)((stack_data_t *)(*pop_data))->datalen), *emp_len_ptr);
			 printf("\n TRACE[13.2]: Emp ID :%u, Emp len: %u", pop_emp->emp_id, pop_emp->name_len);
			 printf("\n TRACE[13.3]: Emp name :%s", ((char *) pop_emp->databuff ));
			 printf("\n TRACE[13.4]: Emp Node addr : 0X%X, Name Addr : 0X%X", pop_emp, pop_emp->databuff);
		 }
		 if((Stack_Emp_Data(POP_DATA_OPER, stack_megaptr, pop_emp)) != TRUE)
			 return FALSE;
	 }
     return TRUE;
 }
 /*******************************************************************
 Function Name  : Stack_Sub_Data(stack_head *stack_megaptr, void *delete_data_ptr  )
 Description    :
 Remarks        :
 Func ID        : 14
 *******************************************************************/
 int Stack_Sub_Data(unsigned char mode, stack_head *stack_megaptr, void *data_ptr)
 {
	stack_data_t *ptr;
	int ret_state = TRUE;

	if(data_ptr == NULL)
	{
		ptr = (stack_data_t *)(stack_megaptr->stack_array + stack_megaptr->top_index)->databuff;
	}
	else
	{
		ptr = (stack_data_t *) data_ptr;
	}
	if(ptr == NULL)
	{
		printf("\n ERR[14.1]: Null Ptr");
		return FALSE;
	}
	switch(ptr->msgtype)
	{
		case EMP_MSG:
		  ret_state = Stack_Emp_Data(mode, stack_megaptr, ptr->databuff);
		break;
		case NUM_MSG:
		  ret_state = Stack_Num_Data(mode, stack_megaptr, ptr->databuff);
		break;
		default:
		    printf("\n ERR[14.2]: invalid msg type - %u", ptr->msgtype);
			free(ptr);
			return FALSE;
	}
	if(mode == POP_DATA_OPER)
	   free(ptr);
	return TRUE;
 }
 /*******************************************************************
 Function Name  : Delete_Stack_Emp_Data(void *delete_data_ptr  )
 Description    :
 Remarks        :
 Func ID        : 15
 *******************************************************************/
 int Delete_Stack_Emp_Data(void *delete_data_ptr)
 {
    emp_info *del_ptr = NULL;
    char *name;

	if(delete_data_ptr == NULL)
	{
		printf("ERR[15.1]: Null Ptr");
		return FALSE;
	}
    del_ptr = (emp_info *)delete_data_ptr;
	name = (char *)del_ptr->databuff;
    if(trace_flag)
	{
		printf("\n TRACE[15.2]: Delete Emp ID: %u, len: %u, name: %s ", del_ptr->emp_id, del_ptr->name_len, name);
	}
	free(del_ptr);
	free(name);
	return TRUE;
 }

 /*******************************************************************
   Function Name : Stack_Emp_Data
   Description   : deals Emp data
   Remarks       :
   fUNC_ID       : 16
 *******************************************************************/
 int Stack_Emp_Data(unsigned char mode, stack_head *stack_megaptr, void *pop_data)
 {
	 emp_info *emp_ptr = (emp_info *) pop_data;
	 int ret_state = FALSE;

	 if(emp_ptr == NULL)
	 {
		 printf("\n ERR[16.1]: Null ptr");
		 return FALSE;
	 }
	 switch(mode)
	 {
		 case PUSH_DATA_OPER:
		    ret_state = Push_EmpData(stack_megaptr, NULL);
		 break;
		 case POP_DATA_OPER:
		     printf("\n Poped Emp ID: %u, Emp name len: %u, name: %s", emp_ptr->emp_id, emp_ptr->name_len, (char *)emp_ptr->databuff);
			 ret_state = Delete_Stack_Emp_Data(pop_data);
         break;
		 case RETRIEVE_NODE:
		 case RETRIEVE_TOP:
		     printf("\n retrieve Emp ID: %u, Emp name len: %u, name: %s", emp_ptr->emp_id, emp_ptr->name_len, (char *)emp_ptr->databuff);
			 ret_state = TRUE;
		 break;
		 default:
             printf("\n Err[16.2]: Invalid Emp oper ");
	 }
	 return ret_state;
 }
/*******************************************************************
  Function Name  : Retrieve_StackData(stack_head *stack_megaptr, stack_node *cur_node, unsigned char *stack_datalen, void **pop_data)
  Description    : retrieves data either at top or intermediate stack node
  Remarks        : doesnt delete its node
  Func ID        : 17
  *******************************************************************/
 int Retrieve_StackData(int ret_mode, stack_head *stack_megaptr, stack_node *cur_node, unsigned char *stack_datalen, void *const *pop_data)
 {
    stack_node *get_node;
	stack_data_t *stack_data_ptr;
	int ret_state = FALSE;
    unsigned char totstack_datalen;
    int retrieve_index, cur_index, node_count, num_nodes;

	if((Empty_Stack(stack_megaptr)) == TRUE || pop_data == NULL)
	{
	  // printf("\n ERR[17.1]: Try in S Head's Ptr: %#X to retrive in empty stack",stack_megaptr);
	   return ret_state;
    }
	/* not a empty stack */
   if((Access_Stack(stack_megaptr)) == FALSE)
       return FALSE;
    Control_Stack(stack_megaptr, NO_ACCESS);
    switch(ret_mode)
    {
       case RETRIEVE_TOP:
	   case RETRIEVE_NODE:
	     if(ret_mode == RETRIEVE_TOP)
		 {
         /* data to be retrieve from stack's top, is located at stack's top_index - 1 */
            get_node = stack_megaptr->stack_array + stack_megaptr->top_index - 1;
			retrieve_index = stack_megaptr->top_index - 1;
		 }
		 else
		 {
			  /* make sure that cur_node points to stack node or empty node */
             if(cur_node == NULL || cur_node > stack_megaptr->stack_array + stack_megaptr->top_index - 1 || cur_node < stack_megaptr->stack_array )
	         {
	             printf("\n ERR[17.3]: Curnode's of S's head addr: %#X, is invalid  ",stack_megaptr );
	             break;
	         }
            get_node = cur_node;
	        retrieve_index = get_node - stack_megaptr->stack_array;
		 }
         stack_data_ptr	=  (stack_data_t *)get_node->databuff;
		 *stack_datalen = stack_data_ptr->datalen;
		 if(trace_flag)
		 {
			printf("\n TRACE[17.4]: Msg type: %u, var data len: %u",  stack_data_ptr->msgtype, stack_data_ptr->datalen);
	        printf("\n TRACE[17.5]: S Head's Addr:%#X, Node Addr: 0X%X, retrieve index[%u]", \
	         stack_megaptr, get_node, retrieve_index);
		    printf("\n TRACE[17.6]: retrieve S node: %#X:: totstack_datalen: %u, Node's stack_databuff: %#X",\
               get_node, *stack_datalen, stack_data_ptr->databuff);
		 }
		 ret_state = Stack_Sub_Data(ret_mode, stack_megaptr, stack_data_ptr);
         break;
	   case RETRIEVE_WHOLE_STACK:
	       /* not a empty stack  */
 	      if((Access_Stack(stack_megaptr)) == FALSE)
             return FALSE;
 	      Control_Stack(stack_megaptr, NO_ACCESS);
 	      /* stack's top most data is located at stack's top_index - 1 */
 	      cur_index = stack_megaptr->top_index - 1;
		  node_count = 0;
		  num_nodes = Stack_Count(stack_megaptr);
 	      /* deleting each node from stack's top */
           while(node_count < num_nodes)
           {
			  /* current stack node & its data buffer */
			  cur_node = stack_megaptr->stack_array + cur_index;
 			  stack_data_ptr = (stack_data_t *)stack_megaptr->stack_array[cur_index].databuff;
			  *stack_datalen = stack_data_ptr->datalen;
			  if(trace_flag)
		      {
				  printf("\n TRACE[17.8]: Msg type: %u, var data len: %u",  stack_data_ptr->msgtype, stack_data_ptr->datalen);
	              printf("\n TRACE[17.9]: S Head's Addr:%#X, Node Addr: 0X%X, retrieve index[%u]", \
	                 stack_megaptr, cur_node, retrieve_index);
		           printf("\n TRACE[17.10]: totstack_datalen: %u, Node's stack_databuff: %#X",\
                      *stack_datalen, stack_data_ptr->databuff);
		       }
 			  Stack_Sub_Data(RETRIEVE_NODE, stack_megaptr, stack_data_ptr);
 			  ++node_count;
 			  --cur_index;
 	      }
		  Control_Stack(stack_megaptr,FREE_ACCESS);
		  ret_state = TRUE;
	   break;
       default:
         printf("\n ERR[17.6]: retrieve mode: %d", ret_mode );
    }
    Control_Stack(stack_megaptr, FREE_ACCESS);
	return ret_state;
 }

 /*******************************************************************
  Function Name  : Destroy_Stack
  Description    : makes the stack empty
  Remarks        :
  Func ID        : 18
  *******************************************************************/
   int Destroy_Stack(unsigned char mode, stack_head *stack_megaptr)
   {
 	  int ret_state = FALSE;
      stack_node *cur_node = NULL;
      stack_data_t *del_buff = NULL;
      char node_count = 0, cur_index;

 	  if((Empty_Stack(stack_megaptr)) == TRUE)
	  {
		   // printf("\n ERR[18.1]: S's Head Ptr: %#X, is already empty to destroy",stack_megaptr);
			return FALSE;
	  }
 	   /* not a empty stack  */
 	      if((Access_Stack(stack_megaptr)) == FALSE)
             return FALSE;
 	      Control_Stack(stack_megaptr, NO_ACCESS);
 	      /* stack's top most data is located at stack's top_index - 1 */
 	      cur_index = stack_megaptr->top_index - 1;
 	      /* deleting each node from stack's top */
           while(node_count < stack_megaptr->stack_count)
           {
			  /* current stack node & its data buffer */
			  cur_node = stack_megaptr->stack_array + cur_index;
 			  del_buff = (stack_data_t *)cur_node->databuff;
 			  /* add codes here, if you want to retrive data from node, before been deleted */
 			  Stack_Sub_Data(POP_DATA_OPER, stack_megaptr, del_buff);
 			  ++node_count;
 			  --cur_index;
 	      }
		  Control_Stack(stack_megaptr,FREE_ACCESS);
		  Initialize_Stack(mode, stack_megaptr);
          ret_state = TRUE;
       return ret_state;
  }

/*******************************************************************
   Function Name : Display_Stack(int mode, void *disp_input)
   Description   : display's stack related info
   Remarks       :
   fUNC_ID       : 19
 *******************************************************************/
int Display_Stack(int mode, stack_head *stack_megaptr)
{
	int ret_state = TRUE;
    unsigned char stack_datalen;
    stack_data_t *pop_data = NULL;
    char node_count = 0, cur_index, num_nodes;
	stack_node *retrieve_stack;

	switch(mode)
	{
	   case STACK_MEGA_HEAD:
	     printf("\n S Head Addr: %#X:: No of nodes: %u, max stack nodes: %u, max_datasize: %u", \
	       stack_megaptr,stack_megaptr->stack_count, stack_megaptr->max_stacksize, stack_megaptr->max_datasize);
	     printf("\n top index: %d, access_stack: %d, stack_array: %#X", \
	       stack_megaptr->top_index, stack_megaptr->access_stack,stack_megaptr->stack_array);
         break;
       case RETRIEVE_TOP:
	     ret_state = GetStack_Data(mode, stack_megaptr, &stack_datalen, &pop_data );
	     break;
	   case RETRIEVE_WHOLE_STACK:
	       if((Empty_Stack(stack_megaptr)) == TRUE)
	       {
	           return FALSE;
	       }
	      retrieve_stack = stack_megaptr->top_stack_node;
		  cur_index = stack_megaptr->top_index - 1;
		  num_nodes = Stack_Count(stack_megaptr);
		  while(node_count < num_nodes)
		  {
			  pop_data = (stack_data_t *)(stack_megaptr->stack_array + cur_index)->databuff;
			  ret_state = Stack_Sub_Data(RETRIEVE_NODE, stack_megaptr, pop_data);
			  --cur_index;
			  ++node_count;
		  }
	   break;
	   default:
	      printf("\n ERR[19.2]: Invalid display mode: %d", mode);
          ret_state = FALSE;
     }
     return ret_state;
}

/*******************************************************************
   Function Name : GetStack_Data(unsigned get_mode, stack_head *stack_megaptr,unsigned char *stack_datalen, stack_node **pop_data )
   Description   : user level deque or retrieve queue's data
   Remarks       : returns either kind of data it possess, or failed queue oper.
   fUNC_ID       : 20
 *******************************************************************/
int GetStack_Data(unsigned get_mode, stack_head *stack_megaptr, unsigned char *stack_datalen, stack_data_t **const pop_data )
{  /* holds starting address of pop data & start_datalen holds pop data's total length */
   int ret_state = TRUE;

   switch(get_mode)
   {
	   case POP_DATA_OPER:
	     if((Pop_Data(stack_megaptr, stack_datalen, (void **) pop_data)) != TRUE)
			 return FALSE;
		 ret_state = Stack_Sub_Data(get_mode, stack_megaptr, (void *)*pop_data);
         break;
       case RETRIEVE_TOP:
          ret_state = Retrieve_StackData(get_mode, stack_megaptr, NULL, stack_datalen, (void * const *)pop_data);
       break;
	   default:
         printf("\n ERR[20.1]: Invalid S's Head Addr: %#X, get mode: %d",stack_megaptr, get_mode);
         ret_state = FALSE;
   }
   return ret_state;
}

/*******************************************************************
  Function Name : StackData_Range(unsigned mode, stack_head *stack_megaptr)
  Description   : process on partial or whole operation in stack's  data
  Remarks       : pop_data has info about either customer or tree node's data
  fUNC_ID       : 23
 *******************************************************************/
 int StackData_Range(unsigned mode, stack_head *stack_megaptr)
 {
	 stack_node *curptr = NULL;         /* local temp ptr contains current node */
	 int ret_state = TRUE;
     char cur_index;
     unsigned char stack_datalen;
     stack_node *pop_data = NULL;
     int node_count = 0;

     if((Empty_Stack(stack_megaptr)) == TRUE)
     {
		// printf("\n ERR[23.1]: S Head's Ptr: %#X, already stack empty",stack_megaptr);
	     return FALSE;
     }
     if((Access_Stack(stack_megaptr)) == FALSE)
        return FALSE;
	 switch(mode)
     {
		 case DESTROY_STACK_OPER:
		 case DESTROY_STACK_ELEMENTS:
		    node_count =  Stack_Count(stack_megaptr);
            printf("\n INFO[23.2]: Before Destroy:: current num of nodes: %u", node_count);
            ret_state = Destroy_Stack(mode, stack_megaptr);
            break;
          default:
            printf("\n ERR[23.3]: Invalid S Head's Addr: %#X, range_mode: %u",stack_megaptr, mode);
            ret_state = FALSE;
     }
	 return ret_state;
 }
/*******************************************************************
 Function Name  : Push_Data_Msg
 Description    :
 Remarks        :
 Func ID        : 24
*******************************************************************/
int Push_Data_Msg(stack_head *stack_megaptr)
{
	int msgtype, ret_state = TRUE;

	printf("\n Push Datas: 1 - Emp, 2 - a number ");
	printf("\n Enter : ");
	scanf("%d", &msgtype);
	switch(msgtype)
	{
		case EMP_MSG:
		  ret_state = Push_EmpData(stack_megaptr, NULL);
		break;
		case NUM_MSG:
		  ret_state = Push_NumData(stack_megaptr, NULL);
		break;
		default:
		  printf("\n Invalid push data");
		  ret_state = FALSE;
	}
	return ret_state;
}

/*******************************************************************
 Function Name  : Push_NumData
 Description    :
 Remarks        :
 Func ID        : 25
*******************************************************************/
int Push_NumData(stack_head *stack_megaptr, stack_data_t *push_num_ptr)
{
	int ret_state = FALSE, num;
    int *push_num = NULL;
    stack_data_t *stack_data_ptr;
    unsigned char stackdata_totlen;

	if(push_num_ptr == NULL)
	{
	    printf("\n Enter a num : ");
        scanf("%d", &num);
       if((stack_data_ptr = (stack_data_t *)calloc (1, sizeof(stack_data_t))) == NULL)
	   {
	      printf("\n ERR[25.1]: no memory to allocate");
	      return FALSE;
       }
	   if((push_num = (int *) calloc(1, sizeof(int))) == NULL)
	   {
	  	 printf("\n Err[25.2]: No memory to alloc ");
	     free(stack_data_ptr);
		 return FALSE;
	   }
	    stack_data_ptr->msgtype = NUM_MSG;
	    stack_data_ptr->datalen = sizeof(int);
	    stack_data_ptr->databuff = push_num;
	    *push_num = num;
	}
	else
	{
		stack_data_ptr = push_num_ptr;
	}
	printf("\n Push Num Data: %d", *push_num);
    if(trace_flag)
	{
	    printf("\n TRACE[25.3]: Push Data - totstack_datalen:%u:: Msgtype:%u", stack_data_ptr->datalen, stack_data_ptr->msgtype);
	    printf("\n Num Node: 0X%X", push_num );
	}
	ret_state = Push_Data(stack_megaptr, stack_data_ptr->datalen, stack_data_ptr);
	return ret_state;
}
/*******************************************************************
 Function Name  : Delete_Stack_Num_Data(void *delete_data_ptr  )
 Description    :
 Remarks        :
 Func ID        : 26
 *******************************************************************/
 int Delete_Stack_Num_Data(void *delete_data_ptr)
 {
    int *del_ptr = NULL;

	if(delete_data_ptr == NULL)
	{
		printf("ERR[26.1]: Null Ptr");
		return FALSE;
	}
    del_ptr = (int *)delete_data_ptr;
    if(trace_flag)
	{
		printf("\n TRACE[26.2]: Deleted Num: %u", *del_ptr);
	}
	free(del_ptr);
	return TRUE;
 }

 /*******************************************************************
   Function Name : Stack_Num_Data
   Description   : deals num data
   Remarks       :
   fUNC_ID       : 27
 *******************************************************************/
 int Stack_Num_Data(unsigned char mode, stack_head *stack_megaptr, void *pop_data)
 {
	 int *num_ptr = (int *) pop_data;
	 int ret_state = FALSE;

	 if(num_ptr == NULL)
	 {
		 printf("\n ERR[27.1]: Null ptr");
		 return FALSE;
	 }
	 switch(mode)
	 {
		 case PUSH_DATA_OPER:
		    ret_state = Push_NumData(stack_megaptr, NULL);
		 break;
		 case POP_DATA_OPER:
		     printf("\n Poped Num: %u", *num_ptr);
			 ret_state = Delete_Stack_Num_Data(pop_data);
         break;
		 case RETRIEVE_NODE:
		 case RETRIEVE_TOP:
		     printf("\n retrieve Num: %u",  *num_ptr);
			 ret_state = TRUE;
		 break;
		 default:
             printf("\n Err[27.2]: Invalid Emp oper ");
	 }
	 return ret_state;
 }
