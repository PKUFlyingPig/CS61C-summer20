# 

include make.def

EXES=hello$(EXE) pi$(EXE) matmul$(EXE) pi_mc$(EXE) prod_cons$(EXE) \
     matmul_recur$(EXE) mandel$(EXE) linked$(EXE)

all: $(EXES)

hello$(EXE): hello.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o hello hello.$(OBJ) $(LIBS)

pi$(EXE): pi.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi pi.$(OBJ) $(LIBS)

pi_mc$(EXE): pi_mc.$(OBJ) random.$(OBJ)
	$(CLINKER) $(OPTFLAGS) -o pi_mc random.$(OBJ) pi_mc.$(OBJ) $(LIBS)

matmul$(EXE): matmul.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o matmul matmul.$(OBJ) $(LIBS)

prod_cons$(EXE): prod_cons.$(OBJ)
	$(CLINKER) $(OPTFLAGS) -o prod_cons prod_cons.$(OBJ) $(LIBS)

matmul_recur$(EXE):  matmul_recur.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o matmul_recur matmul_recur.$(OBJ) $(LIBS)

mandel$(EXE):  mandel.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o mandel mandel.$(OBJ) $(LIBS)

linked$(EXE):  linked.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o linked linked.$(OBJ) $(LIBS)

test: $(EXES)
	$(PRE)hello$(EXE) 
	$(PRE)pi$(EXE) 
	$(PRE)matmul$(EXE) 
	$(PRE)pi_mc$(EXE) 
	$(PRE)prod_cons$(EXE) 
	$(PRE)matmul_recur$(EXE) 
	$(PRE)mandel$(EXE)
	$(PRE)linked$(EXE)

clean:
	$(RM) $(EXES) *.$(OBJ)

.SUFFIXES:
.SUFFIXES: .c .cpp .$(OBJ)

.c.$(OBJ):
	$(CC) $(CFLAGS) -c $<

.cpp.$(OBJ):
	$(CC) $(CFLAGS) -c $<
