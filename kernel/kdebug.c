#include <asm/processor.h>
#include <brunix/stab.h>
#include <brunix/console.h>
#include <string.h>
#include <stdint.h>

#include <brunix/kdebug.h>

#include <brunix/kernel.h>
//#include <brunix/stdlib.h>


extern const struct Stab __STAB_BEGIN__[];	// Beginning of stabs table
extern const struct Stab __STAB_END__[];	// End of stabs table
extern const char __STABSTR_BEGIN__[];		// Beginning of string table
extern const char __STABSTR_END__[];		// End of string table


//FIXME analyse why it is not working as a local variable of print_stack_backtrace()
struct Eipdebuginfo info[1000] __attribute__ ((section (".data")));

static int debuginfo_eip(uintptr_t addr, struct Eipdebuginfo *info2);


/*
 * stab_binsearch(stabs, region_left, region_right, type, addr)
 *
 * Some stab types are arranged in increasing order by instruction address.  For example, N_FUN stabs (stab entries
 * with n_type == N_FUN), which mark functions, and N_SO stabs, which mark source files.
 *
 * Given an instruction address, this function finds the single stab entry of type 'type' that contains that address.
 *
 * The search takes place within the range [*region_left, *region_right].
 * Thus, to search an entire set of N stabs, you might do:
 *
 *      left = 0;
 *      right = N - 1;     // rightmost stab
 *      stab_binsearch(stabs, &left, &right, type, addr);
 *
 * The search modifies *region_left and *region_right to bracket the 'addr'.  *region_left points to the matching stab
 * that contains 'addr', and *region_right points just before the next stab.  If *region_left > *region_right,
 * then 'addr' is not contained in any matching stab.
 *
 * For example, given these N_SO stabs:
 *		Index  Type   Address
 *		0      SO     f0100000
 *		13     SO     f0100040
 *		117    SO     f0100176
 *		118    SO     f0100178
 *		555    SO     f0100652
 *		556    SO     f0100654
 *		657    SO     f0100849
 *	this code:
 *		left = 0, right = 657;
 *		stab_binsearch(stabs, &left, &right, N_SO, 0xf0100184);
 *	will exit setting left = 118, right = 554.
 *
 */
static void stab_binsearch(const struct Stab *stabs, int *region_left, int *region_right, int type, uintptr_t addr) {
    int l = *region_left, r = *region_right, any_matches = 0;

    while (l <= r) {
        int true_m = (l + r) / 2, m = true_m;

        // search for earliest stab with right type
        while (m >= l && stabs[m].n_type != type)
            m--;
        if (m < l) {	// no match in [l, m]
            l = true_m + 1;
            continue;
        }

        // actual binary search
        any_matches = 1;
        if (stabs[m].n_value < addr) {
            *region_left = m;
            l = true_m + 1;
        } else if (stabs[m].n_value > addr) {
            *region_right = m - 1;
            r = m - 1;
        } else {
            // exact match for 'addr', but continue loop to find *region_right
            *region_left = m;
            l = m;
            addr++;
        }
    }

    if (!any_matches)
        *region_right = *region_left - 1;
    else {
        // find rightmost region containing 'addr'
        for (l = *region_right; l > *region_left && stabs[l].n_type != type; l--)
            /* do nothing */;
        *region_left = l;
    }
}

/*
 * debuginfo_eip(addr, info)
 *
 * Fill in the 'info' structure with information about the specified instruction address, 'addr'.  Returns 0 if
 * information was found, and negative if not.  But even if it returns negative it has stored some information into '*info'.
 */
static int debuginfo_eip(uintptr_t addr, struct Eipdebuginfo *info2) {
    const struct Stab *stabs, *stab_end;
    const char *stabstr, *stabstr_end;
    int lfile, rfile, lfun, rfun, lline; //, rline; TODO ver esto

    // Initialize *info
    *info2->eip_file = '?';
    *(info2->eip_file + 1) = '\0';
    info2->eip_line = -1;
    *info2->eip_fn_name = '?';
    *(info2->eip_fn_name + 1) = '\0';
    info2->eip_fn_addr = addr;
    info2->eip_fn_narg = 0;

    stabs = __STAB_BEGIN__;
    stab_end = __STAB_END__;
    stabstr = __STABSTR_BEGIN__;
    stabstr_end = __STABSTR_END__;

    // String table validity checks
    if (stabstr_end <= stabstr || stabstr_end[-1] != 0) {
        return -1;
    }

    // Now we find the right stabs that define the function containing 'eip'.  First, we find the basic source file
    // containing 'eip'. Then, we look in that source file for the function.  Then we look for the line number.

    // Search the entire set of stabs for the source file (type N_SO).
    lfile = 0;
    rfile = (stab_end - stabs) - 1;
    stab_binsearch(stabs, &lfile, &rfile, N_SO, addr);
    if (lfile == 0) {
        printk("Failed reading stabs -> base: %p, addr: %x\n", &stabs, addr);
        return -1;
    }

    // Search within that file's stabs for the function definition (N_FUN).
    lfun = lfile;
    rfun = rfile;

    stab_binsearch(stabs, &lfun, &rfun, N_FUN, addr);

    if (lfun <= rfun) {
        // stabs[lfun] points to the function name in the string table, but check bounds just in case.
        if (stabs[lfun].n_strx < (uintptr_t) (stabstr_end - stabstr)) {
            const char * stab_func_name = stabstr + stabs[lfun].n_strx;
            size_t func_name_len = (size_t) (strfind(stab_func_name, ':') - stab_func_name);     // Ignore stuff after the colon.
            strncpy(info2->eip_fn_name, stab_func_name, func_name_len);
        }
        info2->eip_fn_addr = stabs[lfun].n_value;
        addr -= info2->eip_fn_addr;
        // Search within the function definition for the line number.
        lline = lfun;
//		rline = rfun;
    } else {
        // Couldn't find function stab!  Maybe we're in an assembly file.  Search the whole file for the line number.
        info2->eip_fn_addr = addr;
        lline = lfile;
//		rline = rfile;
    }



    // Search within [lline, rline] for the line number stab.
    // If found, set info->eip_line to the right line number. If not found, return -1.
    //
    // I will leave it unresolved because it is an exercise for the OS course I based this code on.
    lline = lfile;
//    rline = rfile;


    // Search backwards from the line number for the relevant filename stab.
    // We can't just use the "lfile" stab because inlined functions can interpolate code from a different file!
    // Such included source files use the N_SOL stab type.
    while (lline >= lfile
           && stabs[lline].n_type != N_SOL
           && (stabs[lline].n_type != N_SO || !stabs[lline].n_value))
        lline--;
    if (lline >= lfile && stabs[lline].n_strx < (uint32_t) (stabstr_end - stabstr))
        strncpy(info2->eip_file, stabstr + stabs[lline].n_strx, 1000);


    // Set eip_fn_narg to the number of arguments taken by the function,
    // or 0 if there was no containing function.
    if (lfun < rfun)
        for (lline = lfun + 1;
             lline < rfun && stabs[lline].n_type == N_PSYM;
             lline++) {
            info2->eip_fn_narg++;

            info2->fn_params[info2->eip_fn_narg - 1].is_ptr = false;

            struct FunctionParam fn_param = info2->fn_params[info2->eip_fn_narg - 1];

            const char *str = stabstr + stabs[lline].n_strx;
            char *typeptr = strfind(str, ':') + 1;

            strncpy(fn_param.param_name, str, (size_t) (typeptr - str - 1));
            strncpy(info2->fn_params[info2->eip_fn_narg - 1].param_name, fn_param.param_name, 100);

            char * find = strfind(typeptr, '=');
            if (*find != '\0') {
                if (*(find + 1) == '*') {
                    info2->fn_params[info2->eip_fn_narg - 1].is_ptr = true;
                }
            }

        }

    return 0;
}

void print_stack_backtrace(bool ptr_as_string) {

//    int total = stack_backtrace(info);
    int total = stack_backtrace();

    printk("\n\nStack backtrace:\n");
    for (int i = 0; i < total; i++) {

        if (info[i].valid) {

            printk(" %08x:  ", info[i].eip);

            set_fg_color(COLOR_GREEN);
            printk("%s%c", info[i].eip_fn_name, *info[i].eip_fn_name != '?' ? '(' : ' ');

            for (int j = 0; j < info[i].eip_fn_narg; j++) {
                uintptr_t * ebp = (uint32_t*) info[i].ebp;
                if (info[i].fn_params[j].is_ptr && ptr_as_string) {
                    printk("\"%s\"", *(ebp + 2 + j));   //+2 por layout e i para ir subiendo
                } else {
                    printk("%0x", *(ebp + 2 + j));   //+2 por layout e i para ir subiendo
                }

                if (j < info[i].eip_fn_narg - 1)
                    printk(", ");
            }

            char line_number[10];
            if (info[i].eip_line > 0) {
                itoa(info[i].eip_line, line_number, 10);
            } else {
                strncpy(line_number, "?", 1);
            }

            printk("%c", *info[i].eip_fn_name != '?' ? ')' : ' ');
            set_fg_color(COLOR_BLUE);

            if (strstr("/home/phrack/osdev/workspace/", info[i].eip_file)) {
                strncpy(info[i].eip_file, info[i].eip_file + 35, 100);
            }

            printk(" @ %s:%s\n",
                   *info[i].eip_file == '?' ? "?" : info[i].eip_file,
                   line_number);

            set_fg_color(COLOR_LIGHT_GREY);

            ptrdiff_t fn_addr_offset = (ptrdiff_t) (info[i].eip - info[i].eip_fn_addr);
            char offset[100] = "+";
            int hasOffset = 0;
            if (fn_addr_offset > 0) {
                char numberAsStr[10];
                itoa(fn_addr_offset, numberAsStr, 10);
                strncpy(offset + 1, numberAsStr, 10);
                hasOffset = 1;
            }
            printk("  |__  <%s>%s  %ebp=%08x  %s",
                   info[i].eip_fn_name,
                   hasOffset ? offset : "",
                   info[i].ebp,
                   *info[i].eip_fn_name != '?' ? "params=(" : "");

            for (int j=0; j < info[i].eip_fn_narg; j++) {
                printk("%s%s", info[i].fn_params[j].is_ptr ? "*" : "", info[i].fn_params[j].param_name);
                if (j < info[i].eip_fn_narg - 1)
                    printk(", ");
            }

            printk("%s\n", *info[i].eip_fn_name != '?' ? ")" : "");
        }
        else
            printk(" ??\n");
    }
}


//stack_backtrace(int argc, char **argv, struct Trapframe *tf)
//int stack_backtrace(struct Eipdebuginfo infor[]) {
int stack_backtrace(void) {
    int i = 0;


    uint32_t _ebp;
    asm volatile("movl %%ebp,%0" : "=r" (_ebp));
    uintptr_t * ebp = (uint32_t *) _ebp;
//    uint32_t* ebp = (uintptr_t*) read_ebp();


    // skip call to this function in the stacktrace
    ebp = (uint32_t *) *ebp;

    while (ebp) {
        uintptr_t eip = *(ebp+1);// + 0x100000000;

        info[i].eip = eip;
        info[i].ebp = *ebp;

        if (debuginfo_eip(eip, &info[i]) >= 0) {
            info[i].valid = 1;
        } else {
            info[i].valid = 0;
        }

//        printk("\nDEBUG %x %x %s %s %d %x %d %d\n", info[i].eip, info[i].ebp, info[i].eip_fn_name, info[i].eip_file, info[i].eip_line, info[i].eip_fn_addr, info[i].valid, info[i].eip_fn_narg);
        ebp = (uint32_t *) *ebp;
        i++;
    }

    return i;
}
