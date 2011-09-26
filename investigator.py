import copy
import skill

# Extraschooling Enum
EXTRASCHOOLING_NOTAPPLICABLE = 0
EXTRASCHOOLING_FAILED = 1
EXTRASCHOOLING_SUCCESS = 2

class InvestigatorSkill:
    """Class representing a skill that is attached to an Investigator."""
    def __init__(self, skill, current, minimum):
        self.skill = skill
        self.current = current
        if minimum == -1:
            self.minimum = current
        else:
            self.minimum = minimum
        self.occupational = False
        self.checked = False
    
    def GetCopy(self):
        return copy.deepcopy(self)
    
    def GetEmtpyCopy(self):
        skill_copy = InvestiatorSkill(None, self.current, self.minimum)
        skill_copy.checked = self.checked
        skill_copy.occupational = self.occupational
        return skill_copy
    
    def __cmp__(self, investigator_skill):
        skill_type_1 = self.skill.skill_type
        skill_type_2 = investigator_skill.skill.skill_type
        
        if skill_type_1 == skill.SUBSKILL:
            sort_name_1 = self.skill.skill.name
        else:
            sort_name_1 = self.skill.name
            
        if skill_type_2 == skill.SUBSKILL:
            sort_name_2 = investigator_skill.skill.skill.name
        else:
            sort_name_2 = investigator_skill.skill.name
            
        result = cmp(sort_name_1, sort_name_2)
        if result == 0:
            #sort sub items
            if skill_type_1 == skill.SUBSKILL and skill_type_2 == skill.SUBSKILL:
                if len(self.skill.name) == 0:
                    return ((len(investigator_skill.skill.name) == 0) and [0] or [1])[0]
                if len(investigator_skill.skill.name):
                    return -1
                return cmp(self.skill.name, investigator_skill.skill.name)
            # pub parent items before subitems
            if skill_type_1 == skill.SUBSKILL and skill_type_2 == skill.SPECIALSKILL: return 1
            if skill_type_1 == skill.SPECIALSKILL and skill_type_2 == skill.SUBSKILL: return -1
        else:
            return result

class Investigator:
    def __init__(self, rule_file = '', investigator = None):
        if investigator:
            self = copy.deepcopy(investigator)
            return
        self.AllSkillList = []
        self.SkillList = []
        self.FirearmList = []
        self.MeleeList = []
        self.skill_points = 0
        self.interest_points = 0
        self.age = 0
        self.era = skill.ERA_IS1920
        #attributes
        self.app = 0
        self.con = 0
        self.dex = 0
        self.edu = 0
        self.int = 0
        self.pow = 0
        self.siz = 0
        self.str = 0
        #secondary stats
        self.san = 0
        self.hit_points = 0
        self.magic = 0
        self.extra_schooling = EXTRASCHOOLING_NOTAPPLICABLE
        self.rule_file = ''

    # @todo This requres the skill manager! line 163 of Investigator.cpp
    def SetRuleFile(self, rule_file = None):
        if rule_file:
            self.rule_file = rule_file
        else:
            self.rule_file = 'default.rul'
        return True
    
    def CalculateSkillPoints(self):
        pass
    
    def RunIncomeScript(self):
        pass
    
    def RecalculateMinimumSkillValues(self):
        pass
    
    def RollNewStats(self):
        pass
    
    def FindSubskill(self, skill_, skill_index):
        """Looks for the skill_index-th subskill in an investigator skill.
        
        skill is an InvestigatorSkill, not a skill object."""
        if skill_.skill.type != skill.SPECIALSKILL: return
        specialskill = skill_.skill
        if skill_index < 0 or skill_index >= len(specialskill.subskills): return
        
        for investigator_skill in self.AllSkillList:
            if investigator_skill and investigator_skill.skill.type == skill.SUBSKILL:
                if investigator_skill.skill == specialskill.subskills[skill_index] \
                    and investigator_skill.skill.skill == specialskill:
                    return investigator_skill
        return 
    
    def SortSkillList(self):
        AllSkillList.sort()
        SkillList.sort()
        FirearmList.sort()
        MeleeList.sort()
    
    def CalculateCoreStatistics(self):
        pass
    
    # Skill Management
    def AddSkill(self, skill, occupational = FALSE):
        pass
    
    def SetOccupation(self, occupation_name):
        pass
            
    def AddOccupationalSkill(self, skill):
        pass
    
    def RemoveSkill(self, skill):
        pass
    
    def FindSkill(self, skill_name):
        pattern = '/\*?' + skill_name + '\*?/'
        search_regex = re.compile(pattern).match
        matches = [ skill_ for skill_ in AllSkillList if search_regex(skill_.skill.name, re.IGNORECASE)]
        if len(matches):
            return matches[0]
        return None
    
    # @TODO: Do we need this?
    def GetAttributeByName(self, name):
        pass
    
    # Display Information
    def GetDBString(self, str = -1, siz = -1):
        """Gets the damage bonus for this investigator."""
        if str == -1 and siz == -1:
            str = self.str
            siz = self.siz
        if str < 0 or siz < 0: return ""
        db_total = str + siz
        if db_total <= 12:
            return "-1D6"
        elif db_total <= 16:
            return "-1D4"
        elif db_total <= 24:
            return "none"
        elif db_total <= 32:
            return "+1D4"
        else:
            return "+{0}D6".format((db_total - 25) / 16)
        
    
    # @TODO: Assignment operator?  Probably automatic, really.
    
    # Skill Serialization, for export to old versions - may not be needed
    def WriteSkill(self, archive, skill):
        pass
    def ReadSkill(self, archive, occupational, current, minimum):
        pass
    
    def DuplicateInvestigator(self, investigator):
        pass
    
    # This may not be needed because sorting in python does not suck
    def SortSkillList_Partition(self, skill_list, i, j):
        pass
    