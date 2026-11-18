# CS 140 Project 1 — Forum Clarifications

> **Last reviewed:** 2026-11-12 (final forum compilation, supplied instructor replies through Thu Nov 12, 12:05 AM, Asia/Manila).  
> **Purpose:** Faithful, anonymized compilation of CS 140 Project 1 forum clarifications for the group and coding assistants.  
> **Authority:** Refer to the official Project 1 v2 specification and the original UVLe threads for full context. Only instructor replies are authoritative; student questions and notes are not course requirements. This is the final forum compilation as supplied; later specification changes, if any, would still take precedence.  
> **AI use:** This compilation was prepared with AI assistance; declare its use and preserve/export full, unaltered AI session transcripts under `LLM_Logs/` as required by §4.7. The instructor's 2026-11-02 forum answer specifically requests that the generated Markdown file be kept in `LLM_Logs/`.

## Instructions for Codex and contributors

- Read relevant items before planning or modifying the scheduler, syscalls, instrumentation, and tests.
- Apply each ruling only to its stated phase or policy. Distinguish **instructor-confirmed** content from **interpretation or unresolved questions**.
- Where a later instructor reply corrects an earlier one, follow the correction and preserve the history in the relevant entry.
- Do not infer guarantees beyond what the instructor said. When the specification or answer is ambiguous, flag it rather than silently choosing a rule.
- This is a *reference compilation*, not authorization for AI to generate large implementation sections. Group members remain responsible for understanding their code and documenting AI use.
- For the exact wording and provenance, search the UVLe forum by the **thread title and reply timestamp** below; links were not supplied in the pasted export.

## Quick index

| ID | Forum thread | Instructor reply (2026) | Scope |
|---|---|---|---|
| CL-01 | (Phase 2+) Preemption | Oct 27–28; Nov 10, 7:12 PM; Nov 11, 10:58 PM | Phases 2+; wake/fork preemption timing |
| CL-02 | README.md Code Changes Documentation; README clarification | Oct 28, 6:24 PM; Nov 11, 8:22 AM | README content and detail |
| CL-03 | behavior for vanilla RR; Vanilla Scheduling Clarifier | Oct 29, 10:46 PM; Nov 10, 7:18 PM | Vanilla RR and `setburst` |
| CL-04 | set_burst other attribute behavior | Oct 29, 10:48 PM | `setburst` |
| CL-05 | Phase 1 edge case | Oct 29, 10:51 PM | Phase 1; repeated `setburst` |
| CL-06 | [Phase 4] Calculating Virtual Runtime of a Process Group | Oct 30, 4:01 PM | Phase 4+ group accounting |
| CL-07 | multi-CPU clarifications | Oct 30, 4:02 PM; Nov 5, 11:13 AM | All phases/tests; Makefile |
| CL-08 | Clarification for detecting sh and init processes | Oct 31, 9:17 PM | Critical-process refresh |
| CL-09 | [Phases 3-6] Tiebreaking for two processes with equal ERBT | Oct 31, 9:21 PM | Phases 3–6 |
| CL-10 | [AI Usage] Compiling forum posts into `clarifications.md` | Nov 2, 10:46 AM | AI use/document placement |
| CL-11 | [Phase 4] Clarification on preemption conditions | Nov 2, 11:03 AM; Nov 11, 11:18/11:55 PM | Phases 4–6; group/process ties in preemption |
| CL-12 | [Phase 4] Definition of initial processes | Nov 3, 12:43 PM | Phase 4 static group assignments |
| CL-13 | Preemption Clarification | Nov 3, 4:30 PM; Nov 12, 12:05 AM | `setburst` unconditional yield |
| CL-14 | [Phase 6] Tracking I/O Ticks | Nov 5, 5:20 PM | Phase 6 I/O accounting |
| CL-15 | [Phase 4] Tie-breaking Criteria for Groups with Equal Virtual Runtimes | Nov 6, 6:25 AM; Nov 11, 11:18 PM | Group selection and preemption ties |
| CL-16 | [Documentation] Use of DCS Git repository for storing documentation code | Nov 6, 12:28 PM | Repository contents |
| CL-17 | Project Recording Clarification | Nov 7, 10:12 PM | Video documentation |
| CL-18 | schedlog Behavior | Nov 9, 5:45 AM | `schedlog` timer and vanilla policy label |
| CL-19 | Vanilla Scheduling Clarifier | Nov 9, 5:47 AM; Nov 10, 7:18 PM | Invalid-argument consistency; valid `setburst` under vanilla |
| CL-20 | setburst clarification | Nov 9, 6:04 AM; Nov 10, 8:53 PM | Killed flag, immediate context switch, post-kill ticks |
| CL-21 | Occuring on the Same Tick Behavior | Nov 9, 10:57 AM | Tick accounting/preemption order |
| CL-22 | Section 4.3 concerns; Testing & Analysis Clarification | Nov 9, 10:59 AM and 9:21 PM | Testing required in highest declared branch |
| CL-23 | [Phase 5] Effect of setshares() on accumulated group vruntime | Nov 9; Nov 10, 9:43 AM; Nov 11, 11:22 PM | Fractional vruntime; printing precision |

| CL-24 | Bad Argument in QEMU for Scheduling | Nov 9, 8:42 PM | Invalid `SCHEDULING_POLICY` |
| CL-25 | Testing & Analysis Clarification | Nov 9, 9:21 PM | Testing in declared branch |
| CL-26 | Adding arguments for "make qemu" | Oct 22, 3:13 PM; Nov 9, 9:54 PM | Makefile policy selection; clean |
| CL-27 | [Phase 5] Clarification on yielding in setgroup and groupfork | Nov 10, 4:06 PM | `setgroup` versus `groupfork` |
| CL-28 | Test Case Clarification | Nov 10, 7:15/9:00 PM | Other syscalls; lab citations |
| CL-29 | Clarification on README Filename | Nov 11, 8:10 AM | Keep xv6 `README`; add `README.md` |
| CL-30 | [Phases 3-6] Clarifications Regarding results.py | Nov 11, 8:19 AM | Plot generation and incomplete runs |
| CL-31 | README clarification | Nov 9, 8:37 PM; Nov 11, 8:22 AM | Brief README entries |
| CL-32 | Questions on Project Documentation Items | Nov 11, 11:17 AM | PDF items 7/16; cross-reference |
| CL-33 | [Phase 5+] Behavior of setshares() on invalid arguments | Nov 11, 1:55 PM | Invalid `setshares`; `setgroup` validation |
| CL-34 | Total Burst Time of a Child Process | Nov 11, 3:56 PM | Child burst initialization |
| CL-35 | Further clarification on vruntime | Nov 11, 8:51 AM/4:29 PM | Vruntime explicit field and accuracy |
| CL-36 | [Phases 3-6] schedlog format under RR/STCF and printing in test programs | Nov 11, 3:00/10:40 PM | Phase/policy log format; test prints |
| CL-37 | [Phase 3+] runner.sh number of runs | Nov 11, 10:36/10:58 PM | 10 runs; 20 acceptable |
| CL-38 | General clarifications on killing processes | Nov 11, 11:09 PM | Killed flag and group accounting |

## Confirmed clarifications

### CL-01 — Wake/fork preemption checks and safe yielding (Phases 2+)

**Source:** `(Phase 2+) Preemption`, instructor replies Tue Oct 27 12:01 PM; Wed Oct 28 12:00 PM; Tue Nov 10 3:46 PM and 7:12 PM; Wed Nov 11 10:58 PM.

**Instructor-confirmed:**
- Creating or waking a process calls for a **preemption check**, but does **not automatically require a yield** if the newly eligible process does not take precedence.
- The Oct 28 answer says process-level ties are normally resolved during scheduler selection and compares RUNNABLE processes' ERBT against the running process's ERBT (effectively its RBT since its TIQ is zero).
- In the Nov 10 follow-up, the instructor expressly chose **Sequence B**: `wakeup → check for shorter RBT → preempt → next tick`, rather than always waiting for the following timer tick.
- In the Nov 11 follow-up, a student asked whether the check can happen on wake/fork while the actual **yield occurs on a safer path**, such as when ticks increments, to avoid interrupt/locking panics. The instructor answered **“Yes.”** Thus do not assume a yield is safe or mandatory directly inside every `wakeup`/`fork` execution path.

**Reply evolution/caution:** The original Oct 27 answer suggested waiting until the next timer interrupt, but the Nov 10 answer preferred checking upon wakeup, and the Nov 11 answer permitted deferred yielding on a safer path. The earlier Oct 27 reply suggesting unconditional yielding on wakeup was expressly corrected Oct 28. Preserve all three distinctions: *check timing*, *whether preemption is warranted*, and *safe point for yielding*. The replies do not specify one exact kernel mechanism.

**Related:** CL-09 (process tie-break), CL-11 (grouped preemption including ties), CL-21 (tick decrement first), CL-13 (`setburst` always yields).

### CL-02 — Phase README scope and acceptable level of detail

**Sources:** `README.md Code Changes Documentation`, instructor reply Wed Oct 28 6:24 PM; `README clarification`, replies Mon Nov 9 8:37 PM and Wed Nov 11 8:22 AM.

**Instructor-confirmed:** Each phase's `README.md` documents **all deviations from original xv6**, not only the preceding phase's changes. Code snippets and detailed source walkthroughs are **not required** in the README; short per-file descriptions identifying added helpers and modified functions suffice. Reserve in-depth implementation explanation for the highest-phase PDF documentation.

**Example style endorsed in the thread (paraphrased):** For `kernel/proc.c`, note a newly added helper and its purpose, and that `scheduler()` was modified to invoke it. See CL-29 for the separate `README` filename issue.

### CL-03 — Burst tracking and `setburst` under vanilla round robin

**Sources:** `behavior for vanilla RR`, instructor reply Thu Oct 29 10:46 PM; `Vanilla Scheduling Clarifier`, instructor reply Tue Nov 10 7:18 PM.

**Instructor-confirmed:** Under vanilla xv6 scheduling, **burst times need not be tracked or refreshed**, and `ticks_left` / `ticks_consumed` may be printed as **0**. The later instructor reply says **`setburst` should not have an effect under vanilla scheduling**: vanilla is expected to behave like vanilla xv6 except for `schedlog`.

**Important distinction:** CL-19 also confirms that **invalid-argument validation and its consequences** remain consistent across scheduling policies. Thus the lack of a scheduling effect for a **valid** `setburst` in vanilla should not be read as permission to ignore specified invalid-input handling.

**Related:** CL-18 (vanilla's `STCF` log label), CL-19 (argument validation).

### CL-04 — `setburst` resets remaining and consumed values

**Source:** `set_burst other attribute behavior`, instructor reply Thu Oct 29 10:48 PM.

**Instructor-confirmed:** Calling `setburst(burst_ticks)` sets remaining burst to **`burst_ticks`** and resets ticks consumed to **0**, even if the process had already consumed CPU time.

**Reference:** This aligns with spec §3.3. It does **not** say to subtract past consumed CPU ticks from the new burst budget.

### CL-05 — Repeated burst extensions are allowed

**Source:** `Phase 1 edge case`, instructor reply Thu Oct 29 10:51 PM.

**Instructor-confirmed:** A process may repeatedly call `setburst` before exhausting its current burst, thereby indefinitely extending its execution. This is intended, and **no mechanism is required in Phase 1 to prevent it**.

**Boundary:** This does not waive the specified behavior for invalid burst values or for actually exhausting a burst budget.

### CL-06 — Group virtual runtime persistence and type

**Source:** `[Phase 4] Calculating Virtual Runtime of a Process Group`, instructor reply Fri Oct 30 4:01 PM.

**Instructor-confirmed:** “Virtual runtime is maintained and never reset” means the group retains its accumulated CPU tick history and virtual-runtime value even if no processes are currently assigned. The instructor endorsed the **second interpretation** in the question, **not** adding the entire freshly calculated formula to an already accumulated vruntime on every update. `vruntime` should be stored as an **integer**.

**Reference:** Spec §2.5 defines `vruntime_g = total_cpu_ticks_g × 1024 / shares_g`.

**Further clarification:** See CL-23 (Nov 9): changing group shares affects **future CPU ticks only**; preserve past vruntime and account for fractional increments without simply discarding them. Neither instructor reply prescribes a specific accumulator design.

### CL-07 — Single-CPU scope

**Source:** `multi-CPU clarifications`, instructor replies Fri Oct 30 4:02 PM and Thu Nov 5 11:13 AM.

**Instructor-confirmed:** **All phases will use one CPU.** The instructor also confirmed it is acceptable to **modify the Makefile so xv6 runs with `CPUS=1` by default** (Nov 5 reply).

**Reference:** Spec §4.3 requires `CPUS=1` in all test/run scripts. No multi-CPU behavior needs to be inferred from this answer. See CL-22 for which phases must supply the testing/analysis files.

### CL-08 — Identifying `init` and `sh` by name

**Source:** `Clarification for detecting sh and init processes`, instructor reply Sat Oct 31 9:17 PM.

**Instructor-confirmed:** In response to whether comparing process names using `strncmp(p->name, "init", sizeof("init"))` and `strncmp(p->name, "sh", sizeof("sh"))` would suffice, the instructor answered: **“You are free to implement it like that.”**

**Caution:** This is permission to use that approach, **not a detailed correctness guarantee** for every edge case. A student-supplied note said the group already uses `strncmp` in `account_cpu_tick()` and expressed concern about that ambiguity; that note is not an instructor ruling.

**Test consideration (derived):** Test that the actual `init` and `sh` processes are not killed upon budget exhaustion and that unrelated names are not accidentally treated as critical processes.

### CL-09 — ERBT tie-breaking (Phases 3–6)

**Source:** `[Phases 3-6] Tiebreaking for two processes with equal ERBT`, instructor reply Sat Oct 31 9:21 PM.

**Instructor-confirmed:** If multiple processes have equal **ERBT**, use the specification's normal tie-breakers: **earlier creation time**, then **lower PID**. This also applies to selection *within a group* in Phases 4–6.

**Reference:** Spec §§2.3–2.5. Tiebreaking at selection should not be confused with a reason to preempt a running process (CL-01).

### CL-10 — AI compilation and Markdown location

**Source:** `[AI Usage] Compiling forum posts into `clarifications.md``, instructor reply Mon Nov 2 10:46 AM.

**Instructor-confirmed:** AI-assisted compilation of forum clarifications is acceptable **if AI is used only for compilation, not for generating entire chunks of code based on the clarifications**, provided that AI usage is **declared**, **student identifiers are removed**, and the **generated Markdown file is placed inside the `LLM_Logs` folder**.

**Action:** Keep this file as `LLM_Logs/clarifications.md`, rather than a root-level generated copy. Keep unaltered AI transcript exports as separately required by spec §4.7. Student names/IDs were omitted from this compilation; the original pasted export did not include visible names.

### CL-11 — Two-tier preemption and tie-breaking (Phases 4–6)

**Source:** `[Phase 4] Clarification on preemption conditions`, replies Mon Nov 2 11:03 AM; Wed Nov 11 11:18 PM and 11:55 PM.

**Instructor-confirmed examples from Nov 2:**

| Case | Answer |
|---|---|
| Ready process in another group with lower vruntime | **Preemption** |
| Ready process in same group with lower ERBT | **Preemption** |
| Other group has lower process ERBT but **higher group vruntime** | **No preemption** |
| Running group's vruntime rises above another runnable group's | **Preemption** |

**Later explicit clarification:** The preemption check becomes **two-tiered for Phases 4–6**. First compare eligible groups' vruntime; group-level ties select **lower group ID**. Then compare processes within the selected group by ERBT; process ties use **earlier creation time, then lower PID**. The Nov 11 11:55 PM reply states that a running process is preempted if some other ready process takes precedence under this **full two-tier selection mechanism plus tie-breaking**.

**Correction to earlier compilation:** Previous caution that only *strictly lower* group vruntime/ERBT could trigger preemption is **no longer supported** for grouped phases; the later answer includes ties that make a different process the preferred choice. This supersedes earlier uncertainty Q-04/Q-05. Preserve Phase 2–3 behavior separately under CL-01 and CL-09.

**Related:** CL-15 (group ties), CL-09 (process ties), CL-01 (safe wake/fork yielding), CL-13 (`setburst`), CL-23 (accurate vruntime).

### CL-12 — Initial processes are exempt from Phase 4 modulo assignment

**Source:** `[Phase 4] Definition of initial processes`, instructor reply Tue Nov 3 12:43 PM.

**Instructor-confirmed:** For Phase 4, the **initial processes are `init` and `sh`**, and both belong to **Group 0**. The static group assignment using a process's PID modulo the number of groups **only applies to processes other than `init` and `sh`**.

**Reference:** Resolves the apparent tension between spec §2.5 (initial processes in Group 0) and §4.1.4 (static `pid % N_GROUPS` assignment).

**Implementation/test implications (derived):** Verify `init` and `sh` are assigned Group 0 even if their PIDs would otherwise map elsewhere under modulo; test that other processes receive the specified modulo assignment.

### CL-13 — `setburst` always calls `yield()`, including Phase 1

**Source:** `Preemption Clarification`, instructor replies Tue Nov 3 4:30 PM and Thu Nov 12 12:05 AM.

**Instructor-confirmed:** Disregard **the second preemption bullet** in spec §2.3 describing `setburst` relative to another running process; follow **§3.3** instead. `setburst` is voluntary, and **even in non-preemptive Phase 1 it must call `yield()`** after a valid update. External timer-based preemption checks do not apply in Phase 1.

**Final Nov 12 confirmation:** A process calling `setburst` **must yield regardless of whether a different process would be chosen**. Checking first and skipping the `yield()` when no preemption is needed does **not** satisfy this ruling.

**Boundary:** The forum did not expressly address whether a call with invalid arguments reaches the same normal yield path; invalid handling remains specified separately in §3.3 and CL-19.

### CL-14 — I/O ticks correspond to time in `SLEEPING` state (Phase 6)

**Source:** `[Phase 6] Tracking I/O Ticks`, instructor reply Thu Nov 5 5:20 PM.

**Instructor-confirmed:** For Phase 6's I/O-duration accounting, the instructor confirmed that a process's **I/O duration is the number of ticks it has spent in the `SLEEPING` state**.

**Reference:** Spec §4.1.6's group I/O tracking and shares-boosting rules. The original forum question cited the spec's reminder that `sleep(n)` consumes `n` ticks.

**Implementation/test implications (derived):** Verify sleeping time contributes to group I/O tick accounting and that the boost logic observes the specified I/O-to-CPU ratio. The instructor did not supply a detailed tick-accounting algorithm.

### CL-15 — Group ties choose lower group ID; ties can affect grouped preemption

**Sources:** `[Phase 4] Tie-breaking Criteria for Groups with Equal Virtual Runtimes`, reply Fri Nov 6 6:25 AM; `[Phase 4] Clarification on preemption conditions`, reply Wed Nov 11 11:18 PM and 11:55 PM.

**Instructor-confirmed:** Among eligible groups with the same lowest vruntime, choose the **lower group ID**. The later Phase 4–6 preemption ruling (CL-11) says the full two-tier selection mechanism **including tie-breakers** determines whether a different runnable process takes precedence over the running one; do not treat a vruntime tie as automatically forbidding preemption.

**Related:** CL-09 for process ERBT ties, CL-11 for preemption.

### CL-16 — Documentation source files may be stored in DCS Git

**Source:** `[Documentation] Use of DCS Git repository for storing documentation code`, instructor reply Fri Nov 6 12:28 PM.

**Instructor-confirmed:** It is acceptable to store files **other than xv6/project code** in the DCS Git repository; the forum specifically proposed **Typst documentation** as an example.

**Boundary:** This permission does not replace the official README, PDF, or video deliverable requirements and does not change the specified location of AI-generated `clarifications.md` under `LLM_Logs/` (CL-10).

### CL-17 — Separately recorded presentation clips may be combined

**Source:** `Project Recording Clarification`, instructor reply Sat Nov 7 10:12 PM.

**Instructor-confirmed:** Group members **may record their voice-over/video portions separately and combine the individual videos later** into the project recording.

**Reference:** Spec §4.5's video deliverable rules still apply, including individual presentation participation and the stated format/time requirements.

### CL-18 — `schedlog` timer replacement and vanilla policy label

**Source:** `schedlog Behavior`, instructor reply Mon Nov 9 5:45 AM.

**Instructor-confirmed:** When a process calls `schedlog(N)` and another calls `schedlog(M)`, the latter call **overwrites** the prior duration: `schedlog` lasts **M ticks**, not N + M. Under vanilla xv6 round-robin scheduling, the `<policy>` field in `schedlog` is still **`STCF`**, not `RR` or `VANILLA`.

**Reference:** Spec §3.2 describes the phase-specific format and explicitly prescribes `STCF` for vanilla output. CL-03 allows vanilla `ticks_left` and `ticks_consumed` to be zero.

**Test implications (derived):** Activate `schedlog` twice with different durations and verify the later duration takes effect; check the `STCF` policy token in vanilla scheduler logs.

### CL-19 — Consistent invalid-argument behavior across scheduling policies

**Source:** `Vanilla Scheduling Clarifier`, replies Mon Nov 9 5:47 AM and Tue Nov 10 7:18 PM.

**Instructor-confirmed:** Syscall argument validation and the **consequences of invalid arguments** must be consistent across vanilla RR, STCF, and grouped STCF. Separately, **valid `setburst` should not affect vanilla's scheduling/burst enforcement**; vanilla must otherwise work like original xv6 with `schedlog`.

**Per-syscall requirements (spec §§3.3–3.6):** Invalid `setburst`, `setgroup`, and `groupfork` have their specified `-1`/killed consequences; invalid `setshares` returns `-1` **without killing the caller**, as further confirmed in CL-33. Do not conflate error validation with normal scheduling effects.

### CL-20 — Exhausted burst: killed flag, immediate switch, no post-kill accounting

**Source:** `setburst clarification`, instructor replies Mon Nov 9 6:04 AM and Tue Nov 10 8:53 PM.

**Instructor-confirmed:** On burst exhaustion, **mark the process killed**, not by calling `exit()` directly inside the timer interrupt. **Trigger a context switch immediately after marking it killed**. A marked-killed process remains **eligible to be scheduled** so that it can reach normal termination handling. **Do not count CPU ticks consumed after it was marked killed**. CL-38 extends the last rule to group CPU-tick accounting and confirms ordinary xv6 killing handling is sufficient.

**Boundary:** Eligibility for scheduling does not mean it can exceed its budget through normal CPU tick accounting; the late reply specifically excludes post-kill consumption.

### CL-21 — On a timer tick, decrement the running process's remaining burst first

**Source:** `Occuring on the Same Tick Behavior` (forum spelling), instructor reply Mon Nov 9 10:57 AM.

**Instructor-confirmed:** At a CPU tick, **decrement the remaining burst time of the currently running process first**, before determining whether a ready process should preempt it.

**Test implications (derived):** Check accounting and preemption order when a shorter job becomes ready on the same tick, including when the running process's budget reaches zero. Do not infer other interrupt-ordering details not addressed by the reply.

### CL-22 — Testing files: phase applicability and highest declared branch

**Sources:** `Section 4.3 concerns`, reply Mon Nov 9 10:59 AM; `Testing & Analysis Clarification`, reply Mon Nov 9 9:21 PM.

**Instructor-confirmed:** Spec §4.3 testing/analysis is for **Phases 3–6**, not Phases 1–2. **Either** `sanity_check.sh` **or** `sanity_check.py` suffices. Testing and analysis files are **required only in the highest declared branch**; including them in lower branches is optional but can serve as a **safety net** against deductions under §4.4.

**Caution:** The baseline §4.3 commands, CPU configuration and relevant user-test expectations remain applicable. CL-30 explains plot requirements for programs absent from an earlier phase; CL-37 clarifies acceptable run counts.

### CL-23 — `setshares`: historical vruntime, fractional precision and display

**Source:** `[Phase 5] Effect of setshares() on accumulated group vruntime`, instructor replies Mon Nov 9 6:00 AM, 1:40 PM; Tue Nov 10 9:43 AM; Wed Nov 11 11:22 PM.

**Instructor-confirmed:** Changing shares affects **future CPU ticks only**; preserve accumulated vruntime. The implementation must **keep track of fractional vruntime increments** rather than dropping them on every tick. For **printing** vruntime, limit output to **two decimal places**. Either **rounding or truncating** to two decimals is permitted, with some display inaccuracy allowed.

**Detail:** The Nov 11 11:22 PM answer explicitly accepted *either* printing 6.78 or 6.77 for an underlying 6.7777-type result. This **display allowance should not be mistaken for permission to ignore fractional increments in internal accounting**.

**Further clarification:** CL-35 confirms explicit group `vruntime` storage, accumulating each tick under the contemporaneous shares and permissible bounded numerical error in integer arithmetic. The instructor has not mandated a particular fixed-point implementation.

### CL-24 — Invalid scheduling policy values are untested

**Source:** `Bad Argument in QEMU for Scheduling`, instructor reply Mon Nov 9 8:42 PM.

**Instructor-confirmed:** Invalid `SCHEDULING_POLICY` strings (e.g. `6767`) may be handled however the group chooses, including exiting or falling back to vanilla xv6. Only **valid policy choices** (vanilla xv6, STCF, GROUP_STCF) will be tested.

**Boundary:** Do not weaken requirements for valid policies or the default vanilla selection.

### CL-25 — Required analysis is confined to highest declared branch

**Source:** `Testing & Analysis Clarification`, instructor reply Mon Nov 9 9:21 PM.

**Instructor-confirmed:** Testing/analysis files need only exist in the **highest declared phase branch**. Including them in lower branches is optional and recommended as a safeguard if the highest branch fails grading. This also updates CL-22; do not misread “Phases 3+” as “every branch from Phase 3 onward must contain all test files.”

### CL-26 — `make qemu` policy parameter and recompilation via `make clean`

**Source:** `Adding arguments for "make qemu"`, replies Thu Oct 22 3:13 PM and Mon Nov 9 9:54 PM.

**Instructor-confirmed:** Add `SCHEDULING_POLICY` as a `make qemu` parameter; external implementation references are allowed if cited. It is acceptable if switching policies requires `make clean`, **provided that testing/analysis scripts automatically invoke `make clean` when needed**.

**Boundary:** This does not relax the requirement that scripts correctly build and exercise different policies.

### CL-27 — `setgroup` and `groupfork`: limited guidance about yielding

**Source:** `[Phase 5] Clarification on yielding in setgroup and groupfork`, reply Tue Nov 10 4:06 PM.

**Instructor-confirmed wording (paraphrased):** `setgroup` changes the calling process's group without creating or waking a process; `groupfork` behaves like `fork`. The instructor **did not directly say** whether either syscall must call `yield()` or perform an immediate preemption check, leaving that to be derived from their effects and other scheduling rules.

**Do not overstate:** Use CL-01/CL-11 for preemption triggered by process creation, but do **not** claim this reply specifically mandates an unconditional yield in `setgroup` or `groupfork`.

### CL-28 — Other syscall tests encouraged; course-lab citations optional

**Source:** `Test Case Clarification`, instructor replies Tue Nov 10 7:15 PM and 9:00 PM.

**Instructor-confirmed:** Explicitly testing whether *other* syscalls break is **not required**, but strongly **encouraged** because testers may check unlisted cases and broken vanilla `fork()`/`exec()` incur penalties. Citing insights from lab exercises/problem sets is **technically optional**, though welcome in documentation.

**Boundary:** This does not waive required §4.3.2 sanity checks or reference disclosure for other external resources.

### CL-29 — Keep xv6 `README` untouched; add a separate `README.md`

**Source:** `Clarification on README Filename`, instructor reply Wed Nov 11 8:10 AM.

**Instructor-confirmed:** **Create a separate `README.md`** for required changes and **leave xv6's existing `README` file untouched**. The original Makefile may depend on `README` for `fs.img`; renaming or replacing it can cause build failure.

**Related:** CL-02 on README content.

### CL-30 — `results.py` plotting expectations

**Source:** `[Phases 3-6] Clarifications Regarding results.py`, instructor reply Wed Nov 11 8:19 AM.

**Instructor-confirmed (answers in question order):**
1. No prescribed Python plotting library.
2. Plots should be generated/stored per phase, but there is **no naming convention**.
3. Choose an appropriate plot type/aggregation using your **best judgment**.
4. Parsing logs similarly to Lab 2's `parse.py` is **the intended approach**.
5. If a user test program **does not exist in an earlier phase**, it need not appear in that phase's plot.
6. Only include **complete, successful runs**; re-run failures rather than analyzing incomplete logs.

**Boundary:** Follow §4.3.4's metric requirements for each applicable phase; the instructor explicitly relaxed the “all seven programs” interpretation for phases where some programs are absent.

### CL-31 — Brief per-file README descriptions suffice

**Source:** `README clarification`, replies Mon Nov 9 8:37 PM and Wed Nov 11 8:22 AM.

**Instructor-confirmed:** A concise list of file/function changes, such as adding a helper and invoking it from `scheduler()`, is sufficient. **No code snippets are required** in phase READMEs; explanations in depth belong in the highest-phase PDF.

**Cross-reference:** CL-02 consolidates this guidance with the requirement to document every deviation from original xv6.

### CL-32 — PDF documentation items 7 and 16 and reuse of explanations

**Source:** `Questions on Project Documentation Items`, reply Wed Nov 11 11:17 AM.

**Instructor-confirmed:** Item **7 should discuss process initialization only** (the reference to group assignment there was a typo). Explain **group assignment in item 16**. It is acceptable to refer back to an earlier item's detailed explanation rather than repeat all the detail (e.g. ERBT discussed in item 10 and referenced in item 14).

### CL-33 — Invalid `setshares` preserves shares and does not kill

**Source:** `[Phase 5+] Behavior of setshares() on invalid arguments`, reply Wed Nov 11 1:55 PM.

**Instructor-confirmed:** If shares are nonpositive or the group ID is invalid, **leave the group's shares unchanged** and **do not mark the calling process killed** (spec §3.6 requires return `-1`). For `setgroup`, **validate arguments before attempting updates**; the specified invalid-group killed behavior still applies.

**Related:** CL-19 (consistent invalid-argument handling across policies).

### CL-34 — A child does not inherit the parent's modified burst budget

**Source:** `Total Burst Time of a Child Process`, instructor reply Wed Nov 11 3:56 PM.

**Instructor-confirmed:** A child created via `fork()` **does not inherit the parent's total burst time**; §2.2's default new-process burst behavior applies, even when the parent previously called `setburst`.

**Caution:** This forum answer specifically addresses **total burst time**; do not automatically infer every other child's field without reading the spec.

### CL-35 — Vruntime is an explicit accumulating field; integer arithmetic may approximate

**Source:** `Further clarification on vruntime`, instructor replies Wed Nov 11 8:51 AM and 4:29 PM.

**Instructor-confirmed:**
- **Accumulate** group vruntime as a monotonically increasing counter, applying each tick's **shares at the moment the tick occurs**. Example: after 100 ticks at shares 100, then changing to shares 200 and consuming one tick, calculate `(100 × 1024)/100 + (1 × 1024)/200`, not `(101 × 1024)/200`.
- **Keep `vruntime` as an explicit field** of each group. Other supporting integer fields may be used to account for fractional components.
- Make computation and comparison **as accurate as practical with integer arithmetic**. Some inaccuracy is allowed if not too far off (instructor example: 10 versus true 10.5 acceptable; 15 versus 10.5 not acceptable).

**Related:** CL-06 (persistence), CL-23 (fractional accounting and two-decimal display). Do not treat the numerical example as a formal tolerance guarantee.

### CL-36 — `schedlog` format under RR/STCF and output from user tests

**Source:** `[Phases 3-6] schedlog format under RR/STCF and printing in test programs`, replies Wed Nov 11 3:00 PM and 10:40 PM.

**Instructor-confirmed:**
- Under **vanilla RR and STCF**, print **only one Group 0 line**, including when grouped phases are implemented.
- For **Phases 1–3**, vanilla RR and STCF use the specified **`|STCF|0(0,0),...`** format.
- For **Phases 4–6**, **`|GROUP|...` is used only under GROUP_STCF**. Vanilla RR/STCF still print **`|STCF|...`**, not `|GROUP|`, with one Group 0 line.
- **User test programs may print output.**

**Related:** CL-18 (vanilla log label). The reply does not explicitly prescribe all individual field values in the Phase 4–6 STCF single-line output; retain the spec's applicable formatting obligations.

### CL-37 — `runner.sh` normally generates 10 runs; existing 20-run logs permitted

**Source:** `[Phase 3+] runner.sh number of runs`, replies Wed Nov 11 10:36 PM and 10:58 PM.

**Instructor-confirmed:** The `N=20 ./runner.sh` invocation in §4.3 was a **typo**; `runner.sh` is intended to perform **10 runs**. A group that already generated **20 runs** may **retain those logs and plots** without redoing them for only 10 runs.

**Boundary:** Do not infer that all arbitrary run counts are acceptable; this specifically approves 10 intended and existing 20.

### CL-38 — Marking processes killed is sufficient; stop post-kill CPU accounting

**Source:** `General clarifications on killing processes`, reply Wed Nov 11 11:09 PM.

**Instructor-confirmed:** Across the spec's “preemptively/forcibly killing” and invalid-argument cases, **marking the process killed is sufficient**; there is no need to invent additional process-killing machinery. Normal xv6 termination handling can complete afterward. **Do not increment the killed process's ticks consumed, nor its group's consumed CPU ticks, after it is marked killed.**

**Related:** CL-20 separately mandates **an immediate context switch on burst exhaustion**, even though termination happens through normal killed checks.

## Open points and limitations

| ID | Item | Why still unresolved or implementation-dependent |
|---|---|---|
| Q-01 | Complete safety/correctness of name-based detection of `init` and `sh` | CL-08 permits `strncmp`, but does not guarantee every possible naming edge case. |
| Q-06 | Exact placement of wake/fork preemption check and safe yield | CL-01 confirms check-on-event and allows safe-path yielding, but does not prescribe the exact xv6 implementation. |
| Q-07 | Whether `setgroup` needs a preemption check or yield | CL-27 describes its behavior but deliberately leaves the conclusion to implementation reasoning; avoid inventing an explicit syscall rule. |
| Q-08 | Exact fixed-point/remainder scheme and comparison precision for vruntime | CL-23/CL-35 require accounting for fractional ticks with acceptable bounded inaccuracy but specify no exact algorithm. |

## Source provenance and update procedure

- **Input:** User-supplied compiled forum snapshots through Nov 9 plus the final pasted forum export including newly added and bumped threads with instructor replies through Nov 12, 2026, 12:05 AM (Asia/Manila). This is the final user-supplied forum update.
- **No forum URLs provided.** Preserve the thread titles and timestamps above to locate primary sources; do not fabricate links.
- This file paraphrases instructor answers to avoid attributing student questions and interpretations to instructors. The short quotation in CL-08 is reproduced verbatim.
- The supplied forum update is final. If the official project specification is amended independently, verify any newer authoritative requirements against this snapshot. Earlier correction history is retained.
- **Documentation compliance:** Maintain per-branch root `README.md` files and the highest-phase documentation requirements (spec §§4.4–4.5). Include AI references and unaltered session PDF exports with the prescribed `LLM_Logs/<ConversationNumber>_<ToolUsed>_Purpose.pdf` convention (spec §4.7). This Markdown summary does not replace those exports.

## Revision history

| Date | Revision |
|---|---|
| 2026-11-02 | Initial verified compilation of 11 supplied threads; incorporated explicit instructor guidance about putting AI-generated Markdown inside `LLM_Logs/`. |
| 2026-11-08 | Added CL-12–CL-17 from six new threads; updated CL-07 with Nov 5 Makefile permission; resolved the group-selection tie rule (formerly Q-02), added narrower tie/preemption caveat Q-05, and updated Phase 4 preemption caveats. |
| 2026-11-09 | Added CL-18–CL-23 from six new threads; cross-referenced schedlog, validation, tick ordering, and testing rulings; updated CL-06 for future-only shares effects and fractional vruntime accounting; retired Q-03 as clarified while retaining the absence of an instructor-prescribed algorithm. |
| 2026-11-12 | Final forum snapshot: added CL-24–CL-38 (15 newly supplied threads); updated bumped CL-01, CL-02, CL-03, CL-11, CL-13, CL-15, CL-19, CL-20, CL-22 and CL-23; reconciled late preemption/tie, vruntime, schedlog, killing and testing rulings; revised open questions. |
