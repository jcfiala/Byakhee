import copy
import re

# Era Bit Masks
ERA_ISNONE = 0
ERA_IS1890 = 1
ERA_IS1920 = 2
ERA_IS1990 = 4
ERA_ISALL = 7

NO_TYPE = 1
SKILL = 2
FIREARM = 3
MELEE = 4
ANYSKILL = 5
SPECIALSKILL = 6
SUBSKILL = 7
    
class Skill:
    def __init__(self, name='', base='', era_mask=ERA_ISNONE):
        self.name = name
        self.base = base
        self.EraMask = era_mask
        self.type = name and SKILL or NO_TYPE
        if era_mask and len(name) == 0:
            self.type = ANYSKILL
            
    def GetCopy(self):
        return copy.copy(self)
        
    # @TODO: Implement
    def EvaluateBase(self, investigator):
        pass
    
    # @TODO: Implement exception for ERA_ISNONE?
    def IsEra(self, era):
        """Determine if this skill belongs to given era."""
        if (era):
            return era & self.EraMask
        else:
            return ERA_ISNONE
        
    # @TODO Oh, god, get this unit tested.
    def GetBaseString(self, isBase01, showPercent=True):
        """Read in the base skill code from the rule file and return a base
        skill value"""
        result = ''
        regex = '/^(^([\w+]\+?))([\*|F]*)(\%([\w _]+)\%)(\d*)$/'
        match = re.match(regex, self.base)
        if match:
            if match.group(1):
                result += match.group(2)
            if match.group(4):
                result += match.group(5)
            result += match.group(6)
        if int(result) == 0:
            result = (isBase01 and self.base.count('*') == 0) and '01' or '00'
        return result
        
class FirearmSkill(Skill):
    def __init__(self, name='', base='', era_mask=ERA_ISNONE, dmg='', range='', shots='', bullets='', HP='', MAL=''):
        Skill.__init__(self, name, base, era_mask)
        if (era_mask):
            skill.type = FIREARM
        self.dmg = dmg
        self.range = range
        self.shots = shots
        self.bullets = bullets
        self.HP = HP
        self.MAL = MAL
        
class MeleeSkill(Skill):
    def __init__(self, name='', base='', era_mask=ERA_ISNONE, dmg='', attacks='', HP=''):
        Skill.__init__(self, name, base, era_mask)
        if (era_mask):
            skill.type = MELEE
        self.dmg = dmg
        self.attacks = attacks
        self.HP = HP
        
class SpecialSkill(Skill):
    def __init__(self, name, base, era_mask=ERA_ISNONE, SubSkills = 0, CreateSubSkills = True):
        Skill.__init__(self, name, base, era_mask)
        if (era_mask):
            skill.type = SPECIALSKILL
        self.skills = SubSkills
        self.subskills = []
        self.manage_sub_skills = CreateSubSkills
        if CreateSubSkills:
            for i in range(SubSkills):
                self.subskills.append(SubSkill(self, base, era_mask))
        
    def GetCopy(self):
        return copy.deepcopy(self)
        
    def GetEmptyCopy(self):
        return SpecialSkill(self.name, self.base, self.EraMask, self.skills, FALSE)
        
    def GetSubSkillIndex(self, subskill):
        return self.subskills.index(subskill)
        
class SubSkill(Skill):
    def __init__(self, special_skill, base, era_mask):
        self.skill = special_skill
        self.base = base
        self.EraMask = era_mask
        self.name = ''
        self.type = SUBSKILL
        
    def GetCopy(self):
        new_skill = SubSkill(None, self.base, self.EraMask)
        new_skill.name = self.name
        return new_skill
    