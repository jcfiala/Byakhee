#import unittest
import skill

class TestSkillCase():
    def test_create_skill(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_IS1920)
        assert 'Test Skill' == skill_.name
        assert '10' == skill_.base
        assert skill.SKILL == skill_.type
        assert skill.ERA_IS1920 == skill_.EraMask
        
    def test_create_empty_skill(self):
        skill_ = skill.Skill()
        assert '' == skill_.name
        assert '' == skill_.base
        assert skill.NO_TYPE == skill_.type
        assert skill.ERA_ISNONE == skill_.EraMask
        
    def test_is_era_pass(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_IS1920)
        assert skill_.IsEra(skill.ERA_IS1920)
    
    def test_is_era_pass_all_era(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_ISALL)
        assert skill_.IsEra(skill.ERA_IS1890)
        assert skill_.IsEra(skill.ERA_IS1920)
        assert skill_.IsEra(skill.ERA_IS1990)
        
    def test_is_era_fail(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_IS1920)
        assert not skill_.IsEra(skill.ERA_IS1890)
        
    def test_get_base_string_simple(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_IS1920)
        assert skill_.GetBaseString(False, True) == '10'
        
    def test_get_base_string_empty(self):
        skill_ = skill.Skill('Test Skill', '0', skill.ERA_IS1920)
        assert skill_.GetBaseString(False, True) == '00'
        
    def test_get_base_string_empty_double_zero(self):
        skill_ = skill.Skill('Test Skill', '00', skill.ERA_IS1920)
        assert skill_.GetBaseString(False, True) == '00'
    
    def test_get_base_string_empty_base01(self):
        skill_ = skill.Skill('Test Skill', '0', skill.ERA_IS1920)
        assert skill_.GetBaseString(True, True) == '01'
        
    def test_get_base_string_single_digit(self):
        skill_ = skill.Skill('Test Skill', '5', skill.ERA_IS1920)
        assert skill_.GetBaseString(False, True) == '05'
        
    def test_get_base_string_attribute(self):
        skill_ = skill.Skill('Dodge', '^DEX2', skill.ERA_ISALL)
        assert skill_.GetBaseString(False, True) == 'DEXx2'
        
    def test_get_base_string_combined(self):
        skill_ = skill.Skill('AP Mine', "%Conceal%^LUCK", skill.ERA_ISALL)
        assert skill_.GetBaseString(False, True) == 'Conceal+LUCK'
    
    def test_get_base_string_skillname(self):
        skill_ = skill.Skill('.45 Magnum', '%handgun%', skill.ERA_IS1990)
        assert skill_.GetBaseString(False, True) == 'handgun'
        
    def test_get_base_string_unchangeable(self):
        skill_ = skill.Skill('Cthulhu Mythos', '*00', skill.ERA_ISALL)
        assert skill_.GetBaseString(False, True) == '00'
        
    def test_get_base_string_unchangeable_base01(self):
        skill_ = skill.Skill('Cthulhu Mythos', '*00', skill.ERA_ISALL)
        assert skill_.GetBaseString(True, True) == '00'
        
    def test_get_base_string_firearm(self):
        skill_ = skill.Skill('Handgun', 'F25', skill.ERA_ISALL)
        assert skill_.GetBaseString(False, True) == '25'
        
    def test_create_firearm_skill(self):
        skill_ = skill.FirearmSkill('.44 Magnum', '%handgun%', skill.ERA_ISALL, '2D6', '30 yards', '1', '6', '12', '00')
        assert skill_.name == '.44 Magnum'
        assert skill_.base == '%handgun%'
        assert skill_.EraMask == skill.ERA_ISALL
        assert skill_.type == skill.FIREARM
        assert skill_.dmg == '2D6'
        assert skill_.range == '30 yards'
        assert skill_.shots == '1'
        assert skill_.bullets == '6'
        assert skill_.HP == '12'
        assert skill_.MAL == '00'
        
    def test_create_melee_skill(self):
        skill_ = skill.MeleeSkill('Club', '15', skill.ERA_ISALL, '1D6+db', '1', '6')
        assert skill_.name == 'Club'
        assert skill_.base == '15'
        assert skill_.EraMask == skill.ERA_ISALL
        assert skill_.type == skill.MELEE
        assert skill_.dmg == '1D6+db'
        assert skill_.attacks == '1'
        assert skill_.HP == '6'

    def test_create_special_skill(self):
        skill_ = skill.SpecialSkill('Art', '5', skill.ERA_ISALL, 2)
        assert skill_.name == 'Art'
        assert skill_.base == '5'
        assert skill_.type == skill.SPECIALSKILL
        assert skill_.EraMask == skill.ERA_ISALL
        assert skill_.skills == 2
        assert len(skill_.subskills) == 2
        
    def test_create_subskill(self):
        art_skill = skill.SpecialSkill('Art', '5', skill.ERA_ISALL, 2)
        sub1_skill = skill.SubSkill(art_skill)
        sub1_skill.name = 'Sculpture'
        art_skill.subskills[0] = sub1_skill
        assert art_skill.subskills[0].name == 'Sculpture'
        assert art_skill.GetSubSkillIndex(sub1_skill) == 0
        assert sub1_skill.type == skill.SUBSKILL
        assert sub1_skill.base == '5'
        assert sub1_skill.EraMask == skill.ERA_ISALL
        
        