/* This file is automatically generated from wc-checks.sql and .dist_sandbox/subversion-1.9.1/subversion/libsvn_wc/token-map.h.
 * Do not edit this file -- edit the source and rerun gen-make.py */

#define STMT_VERIFICATION_TRIGGERS 0
#define STMT_0_INFO {"STMT_VERIFICATION_TRIGGERS", NULL}
#define STMT_0 \
  "CREATE TEMPORARY TRIGGER no_repository_updates BEFORE UPDATE ON repository " \
  "BEGIN " \
  "  SELECT RAISE(FAIL, 'Updates to REPOSITORY are not allowed.'); " \
  "END; " \
  "CREATE TEMPORARY TRIGGER validation_01 BEFORE INSERT ON nodes " \
  "WHEN NOT ((new.local_relpath = '' AND new.parent_relpath IS NULL) " \
  "          OR (relpath_depth(new.local_relpath) " \
  "              = relpath_depth(new.parent_relpath) + 1)) " \
  "BEGIN " \
  "  SELECT RAISE(FAIL, 'WC DB validity check 01 failed'); " \
  "END; " \
  "CREATE TEMPORARY TRIGGER validation_02 BEFORE INSERT ON nodes " \
  "WHEN NOT new.op_depth <= relpath_depth(new.local_relpath) " \
  "BEGIN " \
  "  SELECT RAISE(FAIL, 'WC DB validity check 02 failed'); " \
  "END; " \
  "CREATE TEMPORARY TRIGGER validation_03 BEFORE INSERT ON nodes " \
  "WHEN NOT ( " \
  "    (new.op_depth = relpath_depth(new.local_relpath)) " \
  "    OR " \
  "    (EXISTS (SELECT 1 FROM nodes " \
  "              WHERE wc_id = new.wc_id AND op_depth = new.op_depth " \
  "                AND local_relpath = new.parent_relpath)) " \
  "  ) " \
  " AND NOT (new.file_external IS NOT NULL AND new.op_depth = 0) " \
  "BEGIN " \
  "  SELECT RAISE(FAIL, 'WC DB validity check 03 failed'); " \
  "END; " \
  "CREATE TEMPORARY TRIGGER validation_04 BEFORE INSERT ON actual_node " \
  "WHEN NOT (new.local_relpath = '' " \
  "          OR EXISTS (SELECT 1 FROM nodes " \
  "                       WHERE wc_id = new.wc_id " \
  "                         AND local_relpath = new.parent_relpath)) " \
  "BEGIN " \
  "  SELECT RAISE(FAIL, 'WC DB validity check 04 failed'); " \
  "END; " \
  ""

#define STMT_STATIC_VERIFY 1
#define STMT_1_INFO {"STMT_STATIC_VERIFY", NULL}
#define STMT_1 \
  "SELECT local_relpath, op_depth, 1, 'Invalid parent relpath set in NODES' " \
  "FROM nodes n WHERE local_relpath != '' " \
  " AND (parent_relpath IS NULL " \
  "      OR NOT (((local_relpath) > (CASE (parent_relpath) WHEN '' THEN '' ELSE (parent_relpath) || '/' END)) AND ((local_relpath) < CASE (parent_relpath) WHEN '' THEN X'FFFF' ELSE (parent_relpath) || '0' END)) " \
  "      OR relpath_depth(local_relpath) != relpath_depth(parent_relpath)+1) " \
  "UNION ALL " \
  "SELECT local_relpath, -1, 2, 'Invalid parent relpath set in ACTUAL' " \
  "FROM actual_node a WHERE local_relpath != '' " \
  " AND (parent_relpath IS NULL " \
  "      OR NOT (((local_relpath) > (CASE (parent_relpath) WHEN '' THEN '' ELSE (parent_relpath) || '/' END)) AND ((local_relpath) < CASE (parent_relpath) WHEN '' THEN X'FFFF' ELSE (parent_relpath) || '0' END)) " \
  "      OR relpath_depth(local_relpath) != relpath_depth(parent_relpath)+1) " \
  "UNION ALL " \
  "SELECT local_relpath, -1, 10, 'No ancestor in ACTUAL' " \
  "FROM actual_node a WHERE local_relpath != '' " \
  " AND NOT EXISTS(SELECT 1 from nodes i " \
  "                WHERE i.wc_id=a.wc_id " \
  "                  AND i.local_relpath=a.parent_relpath) " \
  " AND NOT EXISTS(SELECT 1 from nodes i " \
  "                WHERE i.wc_id=a.wc_id " \
  "                  AND i.local_relpath=a.local_relpath) " \
  "UNION ALL " \
  "SELECT a.local_relpath, -1, 11, 'Bad or Unneeded actual data' " \
  "FROM actual_node a " \
  "LEFT JOIN nodes n on n.wc_id = a.wc_id AND n.local_relpath = a.local_relpath " \
  "   AND n.op_depth = (SELECT MAX(op_depth) from nodes i " \
  "                     WHERE i.wc_id=a.wc_id AND i.local_relpath=a.local_relpath) " \
  "WHERE (a.properties IS NOT NULL " \
  "       AND (n.presence IS NULL " \
  "            OR n.presence NOT IN ('normal', 'incomplete'))) " \
  "   OR (a.changelist IS NOT NULL AND (n.kind IS NOT NULL AND n.kind != 'file')) " \
  "   OR (a.conflict_data IS NULL AND a.properties IS NULL AND a.changelist IS NULL) " \
  " AND NOT EXISTS(SELECT 1 from nodes i " \
  "                WHERE i.wc_id=a.wc_id " \
  "                  AND i.local_relpath=a.parent_relpath) " \
  "UNION ALL " \
  "SELECT local_relpath, op_depth, 20, 'No ancestor in NODES' " \
  "FROM nodes n WHERE local_relpath != '' " \
  " AND file_external IS NULL " \
  " AND NOT EXISTS(SELECT 1 from nodes i " \
  "                WHERE i.wc_id=n.wc_id " \
  "                  AND i.local_relpath=n.parent_relpath " \
  "                  AND i.op_depth <= n.op_depth) " \
  "UNION ALL " \
  "SELECT local_relpath, op_depth, 21, 'Unneeded node data' " \
  "FROM nodes " \
  "WHERE presence NOT IN ('normal', 'incomplete') " \
  "AND (properties IS NOT NULL " \
  "     OR checksum IS NOT NULL " \
  "     OR depth IS NOT NULL " \
  "     OR symlink_target IS NOT NULL " \
  "     OR changed_revision IS NOT NULL " \
  "     OR (changed_date IS NOT NULL AND changed_date != 0) " \
  "     OR changed_author IS NOT NULL " \
  "     OR translated_size IS NOT NULL " \
  "     OR last_mod_time IS NOT NULL " \
  "     OR dav_cache IS NOT NULL " \
  "     OR file_external IS NOT NULL " \
  "     OR inherited_props IS NOT NULL) " \
  "UNION ALL " \
  "SELECT local_relpath, op_depth, 22, 'Unneeded base-deleted node data' " \
  "FROM nodes " \
  "WHERE presence IN ('base-deleted') " \
  "AND (repos_id IS NOT NULL " \
  "     OR repos_path IS NOT NULL " \
  "     OR revision IS NOT NULL) " \
  "UNION ALL " \
  "SELECT local_relpath, op_depth, 23, 'Kind specific data invalid on normal' " \
  "FROM nodes " \
  "WHERE presence IN ('normal', 'incomplete') " \
  "AND (kind IS NULL " \
  "     OR (repos_path IS NULL " \
  "         AND (properties IS NOT NULL " \
  "              OR changed_revision IS NOT NULL " \
  "              OR changed_author IS NOT NULL " \
  "              OR (changed_date IS NOT NULL AND changed_date != 0))) " \
  "     OR (CASE WHEN kind = 'file' AND repos_path IS NOT NULL " \
  "                                   THEN checksum IS NULL " \
  "                                   ELSE checksum IS NOT NULL END) " \
  "     OR (CASE WHEN kind = 'dir' THEN depth IS NULL " \
  "                                  ELSE depth IS NOT NULL END) " \
  "     OR (CASE WHEN kind = 'symlink' THEN symlink_target IS NULL " \
  "                                      ELSE symlink_target IS NOT NULL END)) " \
  "UNION ALL " \
  "SELECT local_relpath, op_depth, 24, 'Invalid op-depth for local add' " \
  "FROM nodes " \
  "WHERE presence IN ('normal', 'incomplete') " \
  "  AND repos_path IS NULL " \
  "  AND op_depth != relpath_depth(local_relpath) " \
  "UNION ALL " \
  "SELECT local_relpath, op_depth, 25, 'Node missing op-depth ancestor' " \
  "FROM nodes n " \
  "WHERE op_depth < relpath_depth(local_relpath) " \
  "  AND file_external IS NULL " \
  "  AND NOT EXISTS(SELECT 1 FROM nodes p " \
  "                 WHERE p.wc_id=n.wc_id AND p.local_relpath=n.parent_relpath " \
  "                   AND p.op_depth=n.op_depth " \
  "                   AND (p.presence IN ('normal', 'incomplete') " \
  "                        OR (p.presence IN ('base-deleted', 'not-present') " \
  "                            AND n.presence = 'base-deleted'))) " \
  "UNION ALL " \
  "SELECT n.local_relpath, n.op_depth, 26, 'Copied descendant mismatch' " \
  "FROM nodes n " \
  "JOIN nodes p " \
  "  ON p.wc_id=n.wc_id AND p.local_relpath=n.parent_relpath " \
  "  AND n.op_depth=p.op_depth " \
  "WHERE n.op_depth > 0 AND n.presence IN ('normal', 'incomplete') " \
  "   AND (n.repos_id != p.repos_id " \
  "        OR n.repos_path != " \
  "           (CASE WHEN (n.parent_relpath) = '' THEN (CASE WHEN (p.repos_path) = '' THEN (n.local_relpath) WHEN (n.local_relpath) = '' THEN (p.repos_path) ELSE (p.repos_path) || '/' || (n.local_relpath) END) WHEN (p.repos_path) = '' THEN (CASE WHEN (n.parent_relpath) = '' THEN (n.local_relpath)  WHEN SUBSTR((n.local_relpath), 1, LENGTH(n.parent_relpath)) = (n.parent_relpath)  THEN CASE WHEN LENGTH(n.parent_relpath) = LENGTH(n.local_relpath) THEN '' WHEN SUBSTR((n.local_relpath), LENGTH(n.parent_relpath)+1, 1) = '/' THEN SUBSTR((n.local_relpath), LENGTH(n.parent_relpath)+2) END END) WHEN SUBSTR((n.local_relpath), 1, LENGTH(n.parent_relpath)) = (n.parent_relpath) THEN CASE WHEN LENGTH(n.parent_relpath) = LENGTH(n.local_relpath) THEN (p.repos_path) WHEN SUBSTR((n.local_relpath), LENGTH(n.parent_relpath)+1, 1) = '/' THEN (p.repos_path) || SUBSTR((n.local_relpath), LENGTH(n.parent_relpath)+1) END END) " \
  "        OR n.revision != p.revision " \
  "        OR p.kind != 'dir' " \
  "        OR n.moved_here IS NOT p.moved_here) " \
  "UNION ALL " \
  "SELECT n.local_relpath, n.op_depth, 27, 'Invalid op-root presence' " \
  "FROM nodes n " \
  "WHERE n.op_depth = relpath_depth(local_relpath) " \
  "  AND presence NOT IN ('normal', 'incomplete', 'base-deleted') " \
  "UNION ALL " \
  "SELECT n.local_relpath, s.op_depth, 28, 'Incomplete shadowing' " \
  "FROM nodes n " \
  "JOIN nodes s ON s.wc_id=n.wc_id AND s.local_relpath=n.local_relpath " \
  " AND s.op_depth = relpath_depth(s.local_relpath) " \
  " AND s.op_depth = (SELECT MIN(op_depth) FROM nodes d " \
  "                   WHERE d.wc_id=s.wc_id AND d.local_relpath=s.local_relpath " \
  "                     AND d.op_depth > n.op_depth) " \
  "WHERE n.presence IN ('normal', 'incomplete') " \
  "  AND EXISTS(SELECT 1 " \
  "             FROM nodes dn " \
  "             WHERE dn.wc_id=n.wc_id AND dn.op_depth=n.op_depth " \
  "               AND dn.presence IN ('normal', 'incomplete') " \
  "               AND (((dn.local_relpath) > (CASE (n.local_relpath) WHEN '' THEN '' ELSE (n.local_relpath) || '/' END)) AND ((dn.local_relpath) < CASE (n.local_relpath) WHEN '' THEN X'FFFF' ELSE (n.local_relpath) || '0' END)) " \
  "               AND dn.file_external IS NULL " \
  "               AND NOT EXISTS(SELECT 1 " \
  "                              FROM nodes ds " \
  "                              WHERE ds.wc_id=n.wc_id AND ds.op_depth=s.op_depth " \
  "                                AND ds.local_relpath=dn.local_relpath)) " \
  "UNION ALL " \
  "SELECT s.local_relpath, s.op_depth, 29, 'Invalid base-delete' " \
  "FROM nodes s " \
  "LEFT JOIN nodes n ON n.wc_id=s.wc_id AND n.local_relpath=s.local_relpath " \
  " AND n.op_depth = (SELECT MAX(op_depth) FROM nodes d " \
  "                   WHERE d.wc_id=s.wc_id AND d.local_relpath=s.local_relpath " \
  "                     AND d.op_depth < s.op_depth) " \
  "WHERE s.presence = 'base-deleted' " \
  "  AND (n.presence IS NULL " \
  "       OR n.presence NOT IN ('normal', 'incomplete') " \
  "       ) " \
  "UNION ALL " \
  "SELECT n.local_relpath, n.op_depth, 30, 'Invalid data for BASE' " \
  "FROM nodes n " \
  "WHERE n.op_depth = 0 " \
  "  AND (n.moved_to IS NOT NULL " \
  "       OR n.moved_here IS NOT NULL) " \
  "UNION ALL " \
  "SELECT d.local_relpath, d.op_depth, 60, 'Moved here without origin' " \
  "FROM nodes d " \
  "WHERE d.op_depth = relpath_depth(d.local_relpath) " \
  "  AND d.moved_here IS NOT NULL " \
  "  AND NOT EXISTS(SELECT 1 FROM nodes s " \
  "                 WHERE s.wc_id = d.wc_id AND s.moved_to = d.local_relpath) " \
  "UNION ALL " \
  "SELECT s.local_relpath, s.op_depth, 61, 'Moved to without target' " \
  "FROM nodes s " \
  "WHERE s.moved_to IS NOT NULL " \
  "  AND NOT EXISTS(SELECT 1 FROM nodes d " \
  "                 WHERE d.wc_id = s.wc_id AND d.local_relpath = s.moved_to " \
  "                   AND d.op_depth = relpath_depth(d.local_relpath) " \
  "                   AND d.moved_here =1 AND d.repos_path IS NOT NULL) " \
  ""

#define WC_CHECKS_SQL_DECLARE_STATEMENTS(varname) \
  static const char * const varname[] = { \
    STMT_0, \
    STMT_1, \
    NULL \
  }

#define WC_CHECKS_SQL_DECLARE_STATEMENT_INFO(varname) \
  static const char * const varname[][2] = { \
    STMT_0_INFO, \
    STMT_1_INFO, \
    {NULL, NULL} \
  }
